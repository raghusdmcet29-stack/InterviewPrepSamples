//
//  BoundedQueue.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef BoundedQueue_h
#define BoundedQueue_h

#pragma once
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include "TransferRequest.h"

class BoundedQueue {
public:
    explicit BoundedQueue(size_t capacity) : capacity_(capacity) {}

    void enqueue(std::optional<TransferRequest> item) {
        std::unique_lock<std::mutex> lk(mu_);
        notFull_.wait(lk, [this] { return items_.size() < capacity_; });
        items_.push_back(item);
        notEmpty_.notify_one();
    }

    std::optional<TransferRequest> dequeue() {
        std::unique_lock<std::mutex> lk(mu_);
        notEmpty_.wait(lk, [this] { return !items_.empty(); });
        auto item = items_.front();
        items_.pop_front();
        notFull_.notify_one();
        return item;
    }

private:
    size_t capacity_;
    std::deque<std::optional<TransferRequest>> items_;
    std::mutex mu_;
    std::condition_variable notFull_;
    std::condition_variable notEmpty_;
};

#endif /* BoundedQueue_h */
