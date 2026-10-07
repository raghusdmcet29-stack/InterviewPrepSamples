//
//  main.cpp
//  BoundedProducerConsumer
//
//  Created by Anussha on 07/10/26.
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class BoundedQueue {
public:
    BoundedQueue(size_t cap) : capacity(cap) {}
    
    void add(int item) {
        std::unique_lock<std::mutex> lock(m);
        while (items.size() == capacity) {
            std::cout << "Queue full, producer waiting\n";
            cv.wait(lock);
        }
        items.push(item);
        std::cout << "Added " << item << ", queue size " << items.size() << "\n";
        cv.notify_one();
    }
    
    int take() {
        std::unique_lock<std::mutex> lock(m);
        while (items.empty()) {
              cv.wait(lock);
        }
        int item = items.front();
        items.pop();
        std::cout << "Took " << item << ", queue size " << items.size() << "\n";
        cv.notify_one();
        return item;
    }
    
    
private:
    std::queue<int> items;
    size_t capacity;
    std::mutex m;
    std::condition_variable cv;
};

BoundedQueue queue(2);

void consumerLoop() {
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        queue.take();
    }
}

int main() {
    std::thread consumer(consumerLoop);
    
    for (int i = 1; i <= 5; i++) {
        queue.add(i);
    }
    
    std::cout << "Producer finished\n";
    consumer.join();
    std::cout << "Done\n";
    return 0;
}
