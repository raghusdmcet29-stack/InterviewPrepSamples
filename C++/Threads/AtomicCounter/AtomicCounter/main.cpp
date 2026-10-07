//
//  main.cpp
//  AtomicCounter
//
//  Created by Anussha on 07/10/26.
//
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

const int threadCount = 4;
const int incrementsPerThread = 1000000;

class MutexCounter {
private:
    std::mutex lock;
    int value = 0;
    
public:
    void increment() {
        std::lock_guard<std::mutex> guard(lock);
        value += 1;
    }

    int read() {
        std::lock_guard<std::mutex> guard(lock);
        return value;
    }
};

class AtomicCounter {
private:
    std::atomic<int> value{0};

public:
    void increment() {
        value.fetch_add(1);
    }

    int read() {
        return value.load();
    }
};

MutexCounter mutexCounter;
AtomicCounter atomicCounter;

void mutexWorker() {
    for (int i = 0; i < incrementsPerThread; i++) {
        mutexCounter.increment();
    }
}

void atomicWorker() {
    for (int i = 0; i < incrementsPerThread; i++) {
        atomicCounter.increment();
    }
}

template <typename Counter>
double runTest(Counter &counter) {
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < threadCount; t++) {
        threads.emplace_back([&counter]() {
            for (int i = 0; i < incrementsPerThread; i++) {
                counter.increment();
            }
        });
    }
    for (std::thread &th : threads) {
        th.join();
    }

    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    MutexCounter mutexCounter;
    double mutexTime = runTest(mutexCounter);
    std::cout << "Mutex counter: " << mutexCounter.read()
              << " (expected " << threadCount * incrementsPerThread << ")" << std::endl;
    std::cout << "Mutex time: " << mutexTime << " seconds" << std::endl;

    AtomicCounter atomicCounter;
    double atomicTime = runTest(atomicCounter);
    std::cout << "Atomic counter: " << atomicCounter.read()
              << " (expected " << threadCount * incrementsPerThread << ")" << std::endl;
    std::cout << "Atomic time: " << atomicTime << " seconds" << std::endl;

    return 0;
}
