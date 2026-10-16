//
//  Summary.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include <pthread.h>
#include "Summary.h"

static void* read_balance(void* arg) {
    BalanceTask* task = (BalanceTask*)arg;
    task->balance = bank_balance_of(task->bank, task->id);
    return NULL;
}

// Starts one thread per account, waits for all of them, returns the total
long long summarize(Bank* bank, const int* ids, BalanceTask* tasks, int count) {
    pthread_t threads[MAX_ACCOUNTS];
    for (int i = 0; i < count; i++) {
        tasks[i].bank = bank;
        tasks[i].id = ids[i];
        pthread_create(&threads[i], NULL, read_balance, &tasks[i]);
    }

    long long total = 0;
    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);   // wait for this task to finish
        total += tasks[i].balance;
    }
    return total;
}
