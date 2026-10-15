//
//  main.cpp
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#include <iostream>
#include <thread>
#include <vector>
#include "Reading.hpp"
#include "BoundedQueue.hpp"
#include "Worker.hpp"

int main() {
    BoundedQueue<Reading> queue(4);

    std::vector<std::thread> workers;
    for (int id = 1; id <= 2; id++) {
        workers.emplace_back([&queue, id] { Worker(id, queue).run(); });
    }

    for (double v : {21.0, 23.0, 22.0}) {
        queue.enqueue(Reading{"sensor-1", v});
        queue.enqueue(Reading{"sensor-2", v + 10});
    }

    queue.close();
    for (auto& t : workers) t.join();
    return 0;
}
