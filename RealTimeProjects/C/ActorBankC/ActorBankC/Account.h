//
//  Account.h
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

typedef struct {
    char id[32];
    int balance;
    int frozen;
    pthread_mutex_t m;
} Account;

void account_init(Account *a, const char *id, int balance);
int account_deposit(Account *a, int amount);
int account_withdraw(Account *a, int amount);
void account_freeze(Account *a);
void account_destroy(Account *a);

#endif
