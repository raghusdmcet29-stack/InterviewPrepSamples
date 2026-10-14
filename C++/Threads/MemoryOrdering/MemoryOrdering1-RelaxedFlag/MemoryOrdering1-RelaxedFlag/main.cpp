//
//  main.cpp
//  MemoryOrdering1-RelaxedFlag
//
//  Created by Anussha on 14/10/26.
//
//MemoryOrdering1-RelaxedFlag, part 2

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
                // wait for the start signal
            }
            data.store(42, std::memory_order_relaxed);
            ready.store(true, std::memory_order_relaxed);
        });

        std::thread reader([&] {
            while (!go.load(std::memory_order_relaxed)) {
                // wait for the start signal
            }
            while (!ready.load(std::memory_order_relaxed)) {
                // wait for the writer
            }
            seen = data.load(std::memory_order_relaxed);
        });

        go.store(true, std::memory_order_relaxed);  // start both threads

        writer.join();
        reader.join();

        if (seen != 42) {
            stale++;
        }
    }

    std::cout << "stale reads: " << stale << " out of " << iterations << "\n";
    return 0;
}



//MemoryOrdering1-RelaxedFlag, part 1
/*
#include <atomic>
#include <iostream>
#include <thread>

int main() {
    const int iterations = 10000;
    int stale = 0;

    for (int i = 0; i < iterations; i++) {
        std::atomic<int> data{0};
        std::atomic<bool> ready{false};
        int seen = -1;

        std::thread writer([&] {
            data.store(42, std::memory_order_relaxed);
            ready.store(true, std::memory_order_relaxed);
        });

        std::thread reader([&] {
            while (!ready.load(std::memory_order_relaxed)) {
                // spin until the flag is set
            }
            seen = data.load(std::memory_order_relaxed);
        });

        writer.join();
        reader.join();

        if (seen != 42) {
            stale++;
        }
    }

    std::cout << "stale reads: " << stale << " out of " << iterations << "\n";
    return 0;
}
*/
