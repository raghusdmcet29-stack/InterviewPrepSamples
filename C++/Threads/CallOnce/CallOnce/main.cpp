//
//  main.cpp
//  CallOnce
//
//  Created by Anussha on 12/10/26.
//

#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

std::once_flag configFlag;
std::string config;

void loadConfig(const std::string& who) {
    std::cout << who << " is loading config\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    config = "settings";
}

std::string getConfig(const std::string& who) {
    std::call_once(configFlag, loadConfig, who);
    return config;
}

int main() {
    std::thread t1(getConfig, "T1");
    std::thread t2(getConfig, "T2");
    std::thread t3(getConfig, "T3");
    t1.join();
    t2.join();
    t3.join();
    std::cout << "Done\n";
}
