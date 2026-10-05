//
//  main.cpp
//  DataRace
//
//  Created by Anussha on 05/10/26.
//

// avoiding data race using Mutex

#include <iostream>
#include <thread>
#include <mutex>

int counter = 0;
std::mutex m;

void work() {
    for (int i = 0; i < 50000; i++) {
        // Version A: manual some thing excepption hapens before unlock it never comes out
   /*     m.lock();
        counter += 1;
        m.unlock();
*/
        // Version B: lock_guard safe to use it will unlock after its scope (guards destructor)
        {
            std::lock_guard<std::mutex> guard(m);
            counter += 1;
        }
    }
}

int main() {
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    std::cout << "Expected: 100000\n";
    std::cout << "Actual: " << counter << "\n";
}

// Data race example
/*
#include <iostream>
#include <thread>

int counter = 0;

void work() {
    for (int i = 0; i < 50000; i++) {
        counter += 1;
    }
}

int main() {
    std::thread t1(work);
    std::thread t2(work);
    t1.join();
    t2.join();
    std::cout << "Expected: 100000\n";
    std::cout << "Actual: " << counter << "\n";
}
*/
// Thread creation and join
/*
#include <iostream>
#include <thread>

void sayHello() {
    std::cout << "Hello from thread\n";
}

int main() {
    std::thread t(sayHello);
    t.join();
    std::cout << "Done\n";
}
*/
