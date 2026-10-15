//
//  main.cpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#include <iostream>
#include <shared_mutex>
#include <thread>
#include "Reading.hpp"
#include "BoundedQueue.hpp"


std::mutex print_m;

int main() {
    BoundedQueue<Reading> queue(2);

    std::thread producer([&queue] {
        for (int i = 1; i <= 3; i++) {
            queue.enqueue(Reading{"sensor-A", i * 10.0});
        }
    });

    std::thread consumer([&queue] {
        while (auto r = queue.dequeue()) {
            std::lock_guard lock(print_m);
            std::cout << "took " << r->deviceID << ": " << r->value << std::endl;
        }
        std::lock_guard lock(print_m);
        std::cout << "consumer stopped" << std::endl;
    });

    producer.join();
    queue.close();
    consumer.join();
    return 0;
}
