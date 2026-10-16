//
//  TokenBucket.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef TokenBucket_h
#define TokenBucket_h

#pragma once
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mutex>

class TokenBucket {
public:
    TokenBucket(double capacity, double refillPerSecond)
        : capacity_(capacity), refillPerSecond_(refillPerSecond),
          tokens_(capacity), last_(std::chrono::steady_clock::now()) {}

    // Blocks until one token is available, then takes it
    void take() {
        std::unique_lock<std::mutex> lk(mu_);
        while (true) {
            refill();
            if (tokens_ >= 1.0) {
                tokens_ -= 1.0;
                return;
            }
            cv_.wait_for(lk, std::chrono::milliseconds(10));
        }
    }

private:
    // Caller must hold mu_
    void refill() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - last_;
        tokens_ = std::min(capacity_, tokens_ + elapsed.count() * refillPerSecond_);
        last_ = now;
    }

    double capacity_;
    double refillPerSecond_;
    double tokens_;
    std::chrono::steady_clock::time_point last_;
    std::mutex mu_;
    std::condition_variable cv_;
};


#endif /* TokenBucket_h */
