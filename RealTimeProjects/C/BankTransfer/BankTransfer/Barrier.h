//
//  Barrier.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Barrier_h
#define Barrier_h

#include <stdio.h>
#pragma once
#include <pthread.h>

typedef struct {
    int parties;
    int waiting;
    pthread_mutex_t mu;
    pthread_cond_t cv;
} Barrier;

void barrier_init(Barrier* b, int parties);
void barrier_wait(Barrier* b);

#endif /* Barrier_h */
