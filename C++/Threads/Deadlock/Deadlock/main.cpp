//
//  main.cpp
//  Deadlock
//
//  Created by Anussha on 05/10/26.
//
// Use the locks in same order to avoid deadlock in threads

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex lock1;
std::mutex lock2;

void threadA() {
    lock1.lock();
    std::cout << "A took Lock 1\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "A wants Lock 2\n";
    lock2.lock();
    std::cout << "A took Lock 2\n";
    lock2.unlock();
    lock1.unlock();
}
// normal way with out scoped lock
/*void threadB() {
    lock1.lock();                                   // CHANGED: was lock2
    std::cout << "B took Lock 1\n";                 // CHANGED
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "B wants Lock 2\n";                // CHANGED
    lock2.lock();                                   // CHANGED: was lock1
    std::cout << "B took Lock 2\n";                 // CHANGED
    lock2.unlock();                                 // CHANGED
    lock1.unlock();                                 // CHANGED
}*/
// another wasy using scoped lock
void threadB() {
    std::scoped_lock both(lock2, lock1);
    std::cout << "B took both\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "B done\n";
}

int main() {
    std::thread a(threadA);
    std::thread b(threadB);
    a.join();
    b.join();
    std::cout << "Done\n";
}

// creates dead lock
/*#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex lock1;
std::mutex lock2;

void threadA() {
    lock1.lock();
    std::cout << "A took Lock 1\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "A wants Lock 2\n";
    lock2.lock();
    std::cout << "A took Lock 2\n";
    lock2.unlock();
    lock1.unlock();
}

void threadB() {
    lock2.lock();
    std::cout << "B took Lock 2\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "B wants Lock 1\n";
    lock1.lock();
    std::cout << "B took Lock 1\n";
    lock1.unlock();
    lock2.unlock();
}

int main() {
    std::thread a(threadA);
    std::thread b(threadB);
    a.join();
    b.join();
    std::cout << "Done\n";
}
*/
