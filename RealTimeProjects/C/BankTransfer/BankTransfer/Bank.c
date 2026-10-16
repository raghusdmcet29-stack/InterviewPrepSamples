//
//  Bank.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include "Bank.h"

static Account* find_account(Bank* b, int id) {
    for (int i = 0; i < b->count; i++) {
        if (b->accounts[i].id == id) return &b->accounts[i];
    }
    return NULL;
}

void bank_init(Bank* b) {
    b->count = 0;
    atomic_init(&b->successCount, 0);
    atomic_init(&b->failedCount, 0);
}

void bank_open(Bank* b, int id, long long balance) {
    Account* a = &b->accounts[b->count++];
    a->id = id;
    a->balance = balance;
    pthread_mutex_init(&a->lock, NULL);
}

long long bank_balance_of(Bank* b, int id) {
    Account* a = find_account(b, id);
    pthread_mutex_lock(&a->lock);
    long long value = a->balance;
    pthread_mutex_unlock(&a->lock);
    return value;
}

int bank_transfer(Bank* b, TransferRequest r) {
    Account* from = find_account(b, r.from);
    Account* to = find_account(b, r.to);
    if (!from || !to) {
        atomic_fetch_add(&b->failedCount, 1);
        return 0;
    }

    // Stage 3: always lock the lower-numbered account first
    Account* first = from->id < to->id ? from : to;
    Account* second = from->id < to->id ? to : from;

    pthread_mutex_lock(&first->lock);
    pthread_mutex_lock(&second->lock);

    if (from->balance < r.amount) {
        pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
        atomic_fetch_add(&b->failedCount, 1);
        return 0;
    }
    from->balance -= r.amount;
    to->balance += r.amount;

    pthread_mutex_unlock(&second->lock);
    pthread_mutex_unlock(&first->lock);

    // Stage 4: atomic success counter
    atomic_fetch_add(&b->successCount, 1);
    return 1;
}
