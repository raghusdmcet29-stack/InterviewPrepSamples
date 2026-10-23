//
//  Account.c
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//

#include <string.h>
#include "Account.h"

void account_init(Account *a, const char *id, int balance) {
    strncpy(a->id, id, sizeof(a->id) - 1);
    a->id[sizeof(a->id) - 1] = '\0';
    a->balance = balance;
    a->frozen = 0;
    pthread_mutex_init(&a->m, NULL);
}

int account_deposit(Account *a, int amount) {
    pthread_mutex_lock(&a->m);
    if (a->frozen) {
        pthread_mutex_unlock(&a->m);
        return 0;
    }
    a->balance += amount;
    pthread_mutex_unlock(&a->m);
    return 1;
}

int account_withdraw(Account *a, int amount) {
    pthread_mutex_lock(&a->m);
    if (amount > a->balance) {
        pthread_mutex_unlock(&a->m);
        return 0;
    }
    a->balance -= amount;
    pthread_mutex_unlock(&a->m);
    return 1;
}

void account_freeze(Account *a) {
    pthread_mutex_lock(&a->m);
    a->frozen = 1;
    pthread_mutex_unlock(&a->m);
}

void account_destroy(Account *a) {
    pthread_mutex_destroy(&a->m);
}
