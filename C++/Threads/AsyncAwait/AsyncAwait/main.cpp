//
//  main.cpp
//  AsyncAwait
//
//  Created by Anussha on 08/10/26.
//

#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int fetchNumber() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 7;
}

int main() {
    std::cout << "Start" << std::endl;
    std::async(std::launch::async, fetchNumber);
    std::async(std::launch::async, fetchNumber);
    std::cout << "Main continues" << std::endl;
    return 0;
}

/*
int main() {
    std::cout << "Start" << std::endl;
    std::future<int> a = std::async(std::launch::async, fetchNumber);
    std::future<int> b = std::async(std::launch::async, fetchNumber);
    int total = a.get() + b.get();
    std::cout << "Got " << total << std::endl;
    return 0;
}
*/
/*
int main() {
    std::cout << "Start" << std::endl;
    std::future<int> f = std::async(std::launch::async, fetchNumber);
    std::cout << "Main continues" << std::endl;
    int result = f.get();
    std::cout << "Result " << result << std::endl;
    return 0;
}*/
