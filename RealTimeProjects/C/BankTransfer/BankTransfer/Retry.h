//
//  Retry.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Retry_h
#define Retry_h

#include <stdio.h>
#pragma once

// An operation returns 1 on success, 0 on failure
typedef int (*RetryOp)(void* ctx);

int with_retry(int max_attempts, long base_delay_ms, RetryOp op, void* ctx);
#endif /* Retry_h */
