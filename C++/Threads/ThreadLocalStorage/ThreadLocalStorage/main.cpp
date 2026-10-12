//
//  main.cpp
//  ThreadLocalStorage
//
//  Created by Anussha on 12/10/26.
//
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::mutex printMutex;

void printLine(const std::string& text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << "\n";
}

thread_local std::string threadName;

void work(const std::string& name) {
    threadName = name;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printLine(name + " reads " + threadName);
}

int main() {
    std::thread t1(work, "T1");
    std::thread t2(work, "T2");
    std::thread t3(work, "T3");
    t1.join();
    t2.join();
    t3.join();
    printLine("Done");
}
