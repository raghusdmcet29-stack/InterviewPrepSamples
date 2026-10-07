//
//  main.cpp
//  DiningPhilosophers
//
//  Created by Anussha on 07/10/26.
//

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

std::mutex forks[3];
std::mutex printMutex;

void printLine(const std::string &text) {
    std::lock_guard<std::mutex> guard(printMutex);
    std::cout << text << "\n";
}
// avoiding dead lock
void philosopher(int id) {
    int left = id;
    int right = (id + 1) % 3;
    int first = std::min(left, right);
    int second = std::max(left, right);
    forks[first].lock();
    printLine("P" + std::to_string(id) + " picked up F" + std::to_string(first));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printLine("P" + std::to_string(id) + " wants F" + std::to_string(second));
    forks[second].lock();
    printLine("P" + std::to_string(id) + " eating");
    forks[second].unlock();
    forks[first].unlock();
}

// dead lock
/*
void philosopher(int id) {
    int left = id;
    int right = (id + 1) % 3;
    forks[left].lock();
    printLine("P" + std::to_string(id) + " picked up F" + std::to_string(left));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printLine("P" + std::to_string(id) + " wants F" + std::to_string(right));
    forks[right].lock();
    printLine("P" + std::to_string(id) + " eating");
    forks[right].unlock();
    forks[left].unlock();
}
*/
int main() {
    std::vector<std::thread> threads;
    for (int id = 0; id < 3; id++) {
        threads.emplace_back(philosopher, id);
    }
    for (std::thread &t : threads) {
        t.join();
    }
    std::cout << "Done\n";
    return 0;
}
