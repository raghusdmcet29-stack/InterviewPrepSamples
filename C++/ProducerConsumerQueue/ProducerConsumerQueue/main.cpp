//
//  main.cpp
//  ProducerConsumerQueue
//
//  Created by Anussha on 26/08/26.
//

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

template<typename T>
class ProducerConsumerQueue {
private:
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    
public:
    void push(T value){
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(value));
            std::cout << "Pushed, size = " << queue.size() << std::endl;
        }
        cv.notify_one();
    }
    
    T pop(){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock,[this] {return !queue.empty();});
        T value = std::move(queue.front());
        queue.pop();
        std::cout << "Popped, size = " << queue.size() << std::endl;
        return value;
    }
    
    
};

void producer(ProducerConsumerQueue<int>& q) {
    for (int i = 1; i <= 5; i++) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        q.push(i);
    }
}

void consumer(ProducerConsumerQueue<int>& q) {
    for (int i = 0; i < 5; i++) {
        int val = q.pop();
        std::cout << "Consumer got: " << val << std::endl;
    }
}

/*int main() {
    ProducerConsumerQueue<int> q;
    q.push(42);
    int val = q.pop();
    std::cout << "Got value: " << val << std::endl;
    return 0;
}
*/
int main() {
    ProducerConsumerQueue<int> q;
    std::thread prod(producer, std::ref(q));
    std::thread cons(consumer, std::ref(q));
    prod.join();
    cons.join();
    return 0;
}
