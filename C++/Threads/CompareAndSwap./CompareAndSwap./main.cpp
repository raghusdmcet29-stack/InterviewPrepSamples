//
//  main.cpp
//  CompareAndSwap.
//
//  Created by Anussha on 13/10/26.
//

#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> value(0);

void addOne() {
    int expected = value.load();
    while (!value.compare_exchange_strong(expected, expected + 1)) {
        // on failure, expected now holds the latest value, so the loop retries with it
    }
}

void worker() {
    for (int i = 0; i < 1000; i++) {
        addOne();
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);
    t1.join();
    t2.join();
    std::cout << "Final value: " << value.load() << "\n";
    return 0;
}



/*
#include <atomic>
#include <iostream>

int main() {
    std::atomic<int> value(5);
    
    int expected = 5;
    
    bool ok1 = value.compare_exchange_strong(expected, 6);
    std::cout << "First CAS: " << ok1 << ", value now " << value.load() << "\n";
    
    expected = 5;
    bool ok2 = value.compare_exchange_strong(expected, 7);
    std::cout << "Second CAS: " << ok2 << ", value now " << value.load() << "\n";
    return 0;
    
}
*/
