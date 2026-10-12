//
//  main.cpp
//  SpinLock
//
//  Created by Anussha on 12/10/26.
//

#include <atomic>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::atomic_flag flag = ATOMIC_FLAG_INIT;
int counter = 0;

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

void spinLock() {
    while (flag.test_and_set()) {
        // spin until the old value was false
    }
}

void spinUnlock() {
    flag.clear();
}

void work(const std::string& name) {
    spinLock();
    counter++;
    printLine(name + " added, counter = " + std::to_string(counter));
    spinUnlock();
}

int main() {
    std::thread t1(work, "T1");
    std::thread t2(work, "T2");
    std::thread t3(work, "T3");
    t1.join();
    t2.join();
    t3.join();
    printLine("Done, final counter = " + std::to_string(counter));
}
