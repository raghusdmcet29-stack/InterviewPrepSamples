//
//  BoundedQueue.hpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef BoundedQueue_h
#define BoundedQueue_h

#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>
#include <optional>
#include <cstddef>

template <typename T>
class BoundedQueue {
public:
    explicit BoundedQueue(std::size_t capacity) : capacity_(capacity) {}
    
    void enqueue(T item) {
         std::unique_lock lock(m_);
         not_full_.wait(lock, [this] { return items_.size() < capacity_; });
        if (closed_) return;
         items_.push_back(std::move(item));
         not_empty_.notify_one();
    }
    
    std::optional<T> dequeue() {
        std::unique_lock lock(m_);
        not_empty_.wait(lock, [this] { return !items_.empty() || closed_; });
        if (items_.empty()) return std::nullopt;
        T item = std::move(items_.front());
        items_.pop_front();
        not_full_.notify_one();
        return item;
    }

    void close() {
        std::lock_guard lock(m_);
        closed_ = true;
        not_empty_.notify_all();
        not_full_.notify_all();
    }
    
private:
    std::size_t capacity_;
    std::deque<T> items_;
    bool closed_ = false;
    std::mutex m_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};




#endif /* BoundedQueue_h */
