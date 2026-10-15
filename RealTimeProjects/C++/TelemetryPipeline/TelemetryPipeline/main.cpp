//
//  main.cpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Reading.hpp"
#include "BoundedQueue.hpp"
#include "StatsStore.hpp"
#include "Worker.hpp"

int main() {
    BoundedQueue<Reading> queue(4);
    StatsStore stats;

    std::vector<std::thread> workers;
    for (int id = 1; id <= 2; id++) {
        workers.emplace_back([&queue, &stats, id] { Worker(id, queue, stats).run(); });
    }

    std::thread dashboard([&stats] {
        for (int tick = 1; tick <= 10; tick++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            for (std::string device : {"sensor-1", "sensor-2"}) {
                if (auto s = stats.snapshot(device)) {
                    std::lock_guard lock(print_m);
                    std::cout << "dashboard tick " << tick << ": " << device
                              << " count=" << s->count << std::endl;
                }
            }
        }
    });

    for (double v : {21.0, 23.0, 22.0}) {
        queue.enqueue(Reading{"sensor-1", v});
        queue.enqueue(Reading{"sensor-2", v + 10});
    }

    queue.close();
    for (auto& t : workers) t.join();
    dashboard.join();

    for (std::string device : {"sensor-1", "sensor-2"}) {
        if (auto s = stats.snapshot(device)) {
            std::cout << device << " count=" << s->count
                      << " min=" << s->min << " max=" << s->max
                      << " avg=" << s->average() << std::endl;
        }
    }
    return 0;
}
