//
//  main.cpp
//  TaskGroup
//
//  Created by Anussha on 12/10/26.
//
// jobs having failur and sucess

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

struct JobResult {
    bool ok;
    std::string name;
};

JobResult job(std::string name, int seconds, bool fail) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    if (fail) {
        std::cout << name << " failed\n";
        return {false, name};
    }
    std::cout << name << " finished\n";
    return {true, name};
}

int main() {
    auto f1 = std::async(std::launch::async, job, std::string("J1"), 3, false);
    auto f2 = std::async(std::launch::async, job, std::string("J2"), 1, true);
    auto f3 = std::async(std::launch::async, job, std::string("J3"), 2, false);

    JobResult r1 = f1.get();
    JobResult r2 = f2.get();
    JobResult r3 = f3.get();

    std::cout << "J1 ok: " << r1.ok << "\n";
    std::cout << "J2 ok: " << r2.ok << "\n";
    std::cout << "J3 ok: " << r3.ok << "\n";
}

// jobs all sucess
/*
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::string job(std::string name, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::cout << name << " finished\n";
    return name;
}

int main() {
    auto f1 = std::async(std::launch::async, job, std::string("J1"), 3);
    auto f2 = std::async(std::launch::async, job, std::string("J2"), 1);
    auto f3 = std::async(std::launch::async, job, std::string("J3"), 2);
    
    std::vector<std::string> results;
    results.push_back(f1.get());
    results.push_back(f2.get());
    results.push_back(f3.get());
    
    std::cout << "Results: ";
    for (const auto& r : results) {
        std::cout << r << " ";
    }
    std::cout << "\n";
}
*/
