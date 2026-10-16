//
//  Retry.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Retry_h
#define Retry_h

#pragma once
#include <chrono>
#include <thread>

// Runs op up to maxAttempts times. Waits baseDelay after the first failure,
// then doubles the wait after each further failure.
template <typename Op>
bool withRetry(int maxAttempts, std::chrono::milliseconds baseDelay, Op op) {
    auto delay = baseDelay;
    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        if (op()) {
            return true;
        }
        if (attempt < maxAttempts) {
            std::this_thread::sleep_for(delay);
            delay *= 2;
        }
    }
    return false;
}

#endif /* Retry_h */
