//
//  Barrier.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Barrier_h
#define Barrier_h

#pragma once
#include <condition_variable>
#include <mutex>

class Barrier {
public:
    explicit Barrier(int parties) : parties_(parties) {}

    void wait() {
        std::unique_lock<std::mutex> lk(mu_);
        ++waiting_;
        if (waiting_ == parties_) {
            cv_.notify_all();
        } else {
            cv_.wait(lk, [this] { return waiting_ == parties_; });
        }
    }

private:
    int parties_;
    int waiting_ = 0;
    std::mutex mu_;
    std::condition_variable cv_;
};


#endif /* Barrier_h */
