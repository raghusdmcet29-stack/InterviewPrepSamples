//
//  Summary.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Summary_h
#define Summary_h

#include <stdio.h>
#pragma once
#include "Bank.h"

typedef struct {
    Bank* bank;
    int id;
    long long balance;   // the thread writes its result here
} BalanceTask;

long long summarize(Bank* bank, const int* ids, BalanceTask* tasks, int count);

#endif /* Summary_h */
