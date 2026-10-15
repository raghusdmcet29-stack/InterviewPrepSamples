//
//  Worker.hpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef Worker_h
#define Worker_h

#pragma once
#include <iostream>
#include <mutex>
#include "Reading.hpp"
#include "BoundedQueue.hpp"

inline std::mutex print_m;

class Worker {
public:
    Worker(int id, BoundedQueue<Reading>& queue) : id_(id), queue_(queue) {}

    void run() {
        while (auto r = queue_.dequeue()) {
            std::lock_guard lock(print_m);
            std::cout << "worker " << id_ << " processed " << r->deviceID << ": " << r->value << std::endl;
        }
        std::lock_guard lock(print_m);
        std::cout << "worker " << id_ << " stopped" << std::endl;
    }

private:
    int id_;
    BoundedQueue<Reading>& queue_;
};

#endif /* Worker_h */
