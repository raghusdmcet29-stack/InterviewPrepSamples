//
//  Retry.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//
#include <time.h>
#include "Retry.h"

int with_retry(int max_attempts, long base_delay_ms, RetryOp op, void* ctx) {
    long delay = base_delay_ms;
    for (int attempt = 1; attempt <= max_attempts; attempt++) {
        if (op(ctx)) return 1;
        if (attempt < max_attempts) {
            struct timespec pause = { delay / 1000, (delay % 1000) * 1000000L };
            nanosleep(&pause, NULL);
            delay *= 2;   // wait twice as long next time
        }
    }
    return 0;
}
