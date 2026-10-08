//
//  main.cpp
//  Semaphore
//
//  Created by Anussha on 08/10/26.
//

#include <chrono>
#include <iostream>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

std::counting_semaphore<10> carPark(2);
std::mutex printMutex;

void printLine(const std::string &text) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << text << std::endl;
}

void car(std::string name) {
    carPark.acquire();
    printLine("Car " + name + " parked");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    printLine("Car " + name + " left");
    carPark.release();
}

int main() {
    std::vector<std::thread> cars;
    for (std::string name : {"A", "B", "C"}) {
        cars.emplace_back(car, name);
    }
    for (std::thread &t : cars) {
        t.join();
    }
    printLine("Done");
    return 0;
}


/*
#include <iostream>
#include <semaphore>

int main() {
    std::counting_semaphore<10> carPark(2);

    carPark.acquire();
    std::cout << "Car A parked" << std::endl;

    carPark.acquire();
    std::cout << "Car B parked" << std::endl;

    return 0;
}
*/
