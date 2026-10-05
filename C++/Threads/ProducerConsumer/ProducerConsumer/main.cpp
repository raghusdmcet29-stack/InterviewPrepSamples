//
//  main.cpp
//  ProducerConsumer
//
//  Created by Anussha on 05/10/26.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;

void consumer() {
    for (int i = 0; i < 3; i++) {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {
            cv.wait(lock);
        }
        int item = q.front();
        q.pop();
        std::cout << "Consumer took " << item << "\n";
    }
}

void producer() {
    for (int item = 1; item <= 3; item++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(m);
        q.push(item);
        std::cout << "Producer added " << item << "\n";
        cv.notify_one();
    }
}

int main() {
    std::thread c(consumer);
    std::thread p(producer);
    c.join();
    p.join();
    std::cout << "Done\n";
}

/*
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex m;
std::condition_variable cv;
bool ready = false;

void waiter() {
    std::unique_lock<std::mutex> lock(m);
    std::cout << "Waiter: waiting\n";
    while (!ready) {
        cv.wait(lock);
    }
    std::cout << "Waiter: woke up\n";
}

int main() {
    std::thread t(waiter);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::unique_lock<std::mutex> lock(m);
        ready = true;
        std::cout << "Main: signalling\n";
        cv.notify_one();
    }
    t.join();
    std::cout << "Done\n";
    
    
}
*/
