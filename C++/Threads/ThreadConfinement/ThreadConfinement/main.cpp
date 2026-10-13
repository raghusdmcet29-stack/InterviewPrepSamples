//
//  main.cpp
//  ThreadConfinement
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
std::queue<int> tasks;

void worker() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        while (tasks.empty()) {
            cv.wait(lock);
        }
        int task = tasks.front();
        tasks.pop();
        lock.unlock();

        if (task == -1) {
            break;
        }
        std::cout << "Running task " << task << "\n";
    }
}

void submit(int task) {
    {
        std::lock_guard<std::mutex> lock(m);
        tasks.push(task);
    }
    cv.notify_one();
}

int main() {
    std::thread t(worker);

    submit(1);
    submit(2);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    submit(3);
    submit(-1);

    t.join();
    std::cout << "Done\n";
    return 0;
}



/*
#include <iostream>
#include <queue>
#include <thread>

void worker() {
    std::queue<int> tasks;
    tasks.push(1);
    tasks.push(2);
    tasks.push(3);

    while (!tasks.empty()) {
        int task = tasks.front();
        tasks.pop();
        std::cout << "Running task " << task << "\n";
    }
}

int main() {
    std::thread t(worker);
    t.join();
    std::cout << "Done\n";
    return 0;
}
*/
