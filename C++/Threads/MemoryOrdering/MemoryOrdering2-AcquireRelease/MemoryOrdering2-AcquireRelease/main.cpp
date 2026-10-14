//
//  main.cpp
//  MemoryOrdering2-AcquireRelease
//
//  Created by Anussha on 14/10/26.
//

#include <atomic>
#include <iostream>
#include <thread>

int main() {
    const int iterations = 10000;
    int stale = 0;

    for (int i = 0; i < iterations; i++) {
        std::atomic<int> data{0};
        std::atomic<bool> ready{false};
        std::atomic<bool> go{false};
        int seen = -1;

        std::thread writer([&] {
            while (!go.load(std::memory_order_relaxed)) {
            }
            data.store(42, std::memory_order_relaxed);
            ready.store(true, std::memory_order_release);   // bell: everything before this is visible
        });

        std::thread reader([&] {
            while (!go.load(std::memory_order_relaxed)) {
            }
            while (!ready.load(std::memory_order_acquire)) {  // listen: guarantees the writer's data
            }
            seen = data.load(std::memory_order_relaxed);
        });

        go.store(true, std::memory_order_relaxed);

        writer.join();
        reader.join();

        if (seen != 42) {
            stale++;
        }
    }

    std::cout << "stale reads: " << stale << " out of " << iterations << "\n";
    return 0;
}
