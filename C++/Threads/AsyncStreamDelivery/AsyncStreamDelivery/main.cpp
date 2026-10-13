//
//  main.cpp
//  AsyncStreamDelivery
//
//  Created by Anussha on 13/10/26.
//

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::queue<int> values;
bool finished = false;

void producer() {
    for (int i = 1; i <= 3; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lock(m);
            values.push(i);
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(m);
        finished = true;
    }
    cv.notify_one();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] { return !values.empty() || finished; });
        if (values.empty() && finished) {
            break;
        }
        int value = values.front();
        values.pop();
        lock.unlock();
        std::cout << "Got " << value << "\n";
    }
    std::cout << "Stream finished\n";
}

int main() {
    std::thread p(producer);
    std::thread c(consumer);
    p.join();
    c.join();
    return 0;
}
