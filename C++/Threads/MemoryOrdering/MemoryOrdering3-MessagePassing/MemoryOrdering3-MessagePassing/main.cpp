//
//  main.cpp
//  MemoryOrdering3-MessagePassing
//
//  Created by Anussha on 14/10/26.
//
#include <atomic>
#include <iostream>
#include <thread>

struct Mailbox {
    int value = 0;               // plain int, protected by the flag
    std::atomic<bool> full{false};
};

int main() {
    Mailbox box;

    std::thread writer([&] {
        box.value = 42;                                  // write the data
        box.full.store(true, std::memory_order_release); // publish it
    });

    std::thread reader([&] {
        while (!box.full.load(std::memory_order_acquire)) {
            // wait until the writer publishes
        }
        std::cout << "received: " << box.value << "\n";
    });

    writer.join();
    reader.join();
    return 0;
}
