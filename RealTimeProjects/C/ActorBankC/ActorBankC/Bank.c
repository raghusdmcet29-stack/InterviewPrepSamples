//
//  Bank.c
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//

#include <string.h>
#include "Bank.h"
#include "Journal.h"

void bank_init(Bank *b, const char *journal_path) {
    b->count = 0;
    strncpy(b->journal_path, journal_path, sizeof(b->journal_path) - 1);
    b->journal_path[sizeof(b->journal_path) - 1] = '\0';
    b->next_seq = journal_next_sequence(journal_path);
    pthread_mutex_init(&b->m, NULL);
}

static Account *find_account(Bank *b, const char *id) {
    for (int i = 0; i < b->count; i++) {
        if (strcmp(b->accounts[i].id, id) == 0) {
            return &b->accounts[i];
        }
    }
    return NULL;
}

int bank_open(Bank *b, const char *id, int balance) {
    pthread_mutex_lock(&b->m);
    if (b->count >= MAX_ACCOUNTS) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    if (!journal_append_open(b->journal_path, id, balance)) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    account_init(&b->accounts[b->count], id, balance);
    b->count++;
    pthread_mutex_unlock(&b->m);
    return 1;
}

int bank_balance(Bank *b, const char *id, int *out) {
    pthread_mutex_lock(&b->m);
    Account *a = find_account(b, id);
    if (a == NULL) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    *out = a->balance;
    pthread_mutex_unlock(&b->m);
    return 1;
}

int bank_deposit(Bank *b, const char *id, int amount) {
    pthread_mutex_lock(&b->m);
    Account *a = find_account(b, id);
    if (a == NULL || a->frozen) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    if (!journal_append(b->journal_path, b->next_seq, id, amount)) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    b->next_seq++;
    int ok = account_deposit(a, amount);
    pthread_mutex_unlock(&b->m);
    return ok;
}

int bank_transfer(Bank *b, const char *from, const char *to, int amount) {
    pthread_mutex_lock(&b->m);
    Account *f = find_account(b, from);
    Account *t = find_account(b, to);
    if (f == NULL || t == NULL) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    if (t->frozen || f->balance < amount) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    account_withdraw(f, amount);
    account_deposit(t, amount);
    pthread_mutex_unlock(&b->m);
    return 1;
}

int bank_freeze(Bank *b, const char *id) {
    pthread_mutex_lock(&b->m);
    Account *a = find_account(b, id);
    if (a == NULL) {
        pthread_mutex_unlock(&b->m);
        return 0;
    }
    account_freeze(a);
    pthread_mutex_unlock(&b->m);
    return 1;
}

void bank_destroy(Bank *b) {
    for (int i = 0; i < b->count; i++) {
        account_destroy(&b->accounts[i]);
    }
    pthread_mutex_destroy(&b->m);
}

void bank_load(Bank *b) {
    char ids[MAX_ACCOUNTS][32];
    int balances[MAX_ACCOUNTS];
    int n = journal_rebuild(b->journal_path, ids, balances, MAX_ACCOUNTS);

    pthread_mutex_lock(&b->m);
    for (int i = 0; i < n; i++) {
        account_init(&b->accounts[b->count], ids[i], balances[i]);
        b->count++;
    }
    pthread_mutex_unlock(&b->m);
}
