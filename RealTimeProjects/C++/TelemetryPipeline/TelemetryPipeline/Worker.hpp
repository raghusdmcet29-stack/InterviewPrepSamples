//
//  Worker.hpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef Worker_h
#define Worker_h

#pragma once
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include "Reading.hpp"
#include "BoundedQueue.hpp"
#include "StatsStore.hpp"

inline std::mutex print_m;

class Worker {
public:
    Worker(int id, BoundedQueue<Reading>& queue, StatsStore& stats)
        : id_(id), queue_(queue), stats_(stats) {}

    void run() {
        while (auto r = queue_.dequeue()) {
            {
                std::lock_guard lock(print_m);
                std::cout << "worker " << id_ << " processed " << r->deviceID << ": " << r->value << std::endl;
            }
            stats_.record(*r);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::lock_guard lock(print_m);
        std::cout << "worker " << id_ << " stopped" << std::endl;
    }

private:
    int id_;
    BoundedQueue<Reading>& queue_;
    StatsStore& stats_;
};

#endif /* Worker_h */
