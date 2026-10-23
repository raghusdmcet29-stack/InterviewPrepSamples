//
//  Bank.h
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//

#ifndef BANK_H
#define BANK_H

#include <pthread.h>
#include "Account.h"

#define MAX_ACCOUNTS 10

typedef struct {
    Account accounts[MAX_ACCOUNTS];
    int count;
    char journal_path[256];
    int next_seq;
    pthread_mutex_t m;
} Bank;

void bank_init(Bank *b, const char *journal_path);
int bank_open(Bank *b, const char *id, int balance);
int bank_balance(Bank *b, const char *id, int *out);
int bank_deposit(Bank *b, const char *id, int amount);
int bank_transfer(Bank *b, const char *from, const char *to, int amount);
int bank_freeze(Bank *b, const char *id);
void bank_destroy(Bank *b);
void bank_load(Bank *b);
#endif
