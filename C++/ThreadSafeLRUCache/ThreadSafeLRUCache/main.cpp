//
//  main.cpp
//  ThreadSafeLRUCache
//
//  Created by Anussha on 01/09/26.
//

#include <iostream>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <vector>

template<typename Key, typename Value>
class LRUCache {
private:
    struct Node {
        Key key;
        Value value;
        Node* prev;
        Node* next;
        Node(Key k, Value v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    size_t capacity;
    std::unordered_map<Key, Node*> map;
    Node* head; // most recently used
    Node* tail; // least recently used
    mutable std::mutex mtx;

public:
    LRUCache(size_t cap) : capacity(cap), head(nullptr), tail(nullptr) {}

    ~LRUCache() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
    
private:
    void remove(Node* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;

        node->prev = nullptr;
        node->next = nullptr;
    }

    void insertAtHead(Node* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;

        if (!tail) tail = node;
    }
    
public:
    bool get(const Key& key, Value& outValue) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = map.find(key);
        if (it == map.end()) return false;

        Node* node = it->second;
        remove(node);
        insertAtHead(node);

        outValue = node->value;
        return true;
    }

    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = map.find(key);
        if (it != map.end()) {
            Node* existing = it->second;
            existing->value = value;
            remove(existing);
            insertAtHead(existing);
            return;
        }

        Node* node = new Node(key, value);
        map[key] = node;
        insertAtHead(node);

        if (map.size() > capacity) {
            Node* lru = tail;
            remove(lru);
            map.erase(lru->key);
            delete lru;
        }
    }
};

int main() {
    LRUCache<std::string, int> cache(2);

    cache.put("A", 1);
    cache.put("B", 2);

    int val;
    bool found = cache.get("A", val);
    std::cout << "get(A) mid-trace: " << (found ? std::to_string(val) : "missing") << std::endl; // expect 1

    cache.put("C", 3);

    found = cache.get("B", val);
    std::cout << "get(B) after eviction: " << (found ? std::to_string(val) : "missing") << std::endl; // expect missing

    found = cache.get("A", val);
    std::cout << "get(A) still present: " << (found ? std::to_string(val) : "missing") << std::endl; // expect 1

    found = cache.get("C", val);
    std::cout << "get(C) still present: " << (found ? std::to_string(val) : "missing") << std::endl; // expect 3

    // Concurrent stress test
    LRUCache<int, int> bigCache(50);
    std::vector<std::thread> threads;
    for (int i = 0; i < 1000; ++i) {
        threads.emplace_back([&bigCache, i]() {
            bigCache.put(i % 100, i);
            int dummy;
            bigCache.get(i % 100, dummy);
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "Concurrent stress test completed without crashing" << std::endl;

    return 0;
}
