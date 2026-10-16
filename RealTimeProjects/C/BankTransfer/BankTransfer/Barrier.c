//
//  Barrier.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include "Barrier.h"

void barrier_init(Barrier* b, int parties) {
    b->parties = parties;
    b->waiting = 0;
    pthread_mutex_init(&b->mu, NULL);
    pthread_cond_init(&b->cv, NULL);
}

void barrier_wait(Barrier* b) {
    pthread_mutex_lock(&b->mu);
    b->waiting++;
    if (b->waiting == b->parties) {
        pthread_cond_broadcast(&b->cv);
    } else {
        while (b->waiting < b->parties) {
            pthread_cond_wait(&b->cv, &b->mu);
        }
    }
    pthread_mutex_unlock(&b->mu);
}
