//
//  Bank.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Bank_h
#define Bank_h
#pragma once
#include <pthread.h>
#include <stdatomic.h>

#define MAX_ACCOUNTS 10

typedef struct {
    int from;
    int to;
    long long amount;
} TransferRequest;

typedef struct {
    int id;
    long long balance;
    pthread_mutex_t lock;   // only one thread changes this account at a time
} Account;

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int count;
    atomic_llong successCount;
    atomic_llong failedCount;
} Bank;

void bank_init(Bank* b);
void bank_open(Bank* b, int id, long long balance);
long long bank_balance_of(Bank* b, int id);
int bank_transfer(Bank* b, TransferRequest r);

#endif /* Bank_h */
