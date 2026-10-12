//
//  main.cpp
//  Concurrency12-Barrier
//
//  Created by Anussha on 12/10/26.
//
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

class Barrier {
    std::mutex m;
    std::condition_variable cv;
    int total;
    int arrived = 0;

public:
    explicit Barrier(int total) : total(total) {}

    void wait() {
        std::unique_lock<std::mutex> lock(m);
        arrived++;
        if (arrived == total) {
            cv.notify_all();
        } else {
            while (arrived < total) {
                cv.wait(lock);
            }
        }
    }
};

void worker(std::string name, int seconds, Barrier& barrier) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    printLine(name + " arrived at barrier");
    barrier.wait();
    printLine(name + " passed the barrier");
}

int main() {
    Barrier barrier(3);
    std::thread a(worker, "A", 1, std::ref(barrier));
    std::thread b(worker, "B", 2, std::ref(barrier));
    std::thread c(worker, "C", 3, std::ref(barrier));
    a.join();
    b.join();
    c.join();
    printLine("Done");
}
