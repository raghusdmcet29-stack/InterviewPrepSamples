//
//  main.cpp
//  MemoryPools
//
//  Created by Anussha on 30/09/26.
//
#include <iostream>
#include <vector>

class IntPool {
public:
    int capacity;
    int* block;
    int nextFree = 0;
    std::vector<int> returned;

    IntPool(int cap) {
        capacity = cap;
        block = new int[capacity];
        std::cout << "Pool created: " << capacity << " slots" << std::endl;
    }

    int* allocateSlot() {
        if (!returned.empty()) {
            int index = returned.back();
            returned.pop_back();
            return block + index;
        }
        if (nextFree >= capacity) {
            return nullptr;
        }
        int* slot = block + nextFree;
        nextFree++;
        return slot;
    }

    void giveBack(int* slot) {
        int index = static_cast<int>(slot - block);
        returned.push_back(index);
    }

    ~IntPool() {
        delete[] block;
        std::cout << "Pool freed" << std::endl;
    }
};

int main() {
    IntPool pool(4);

    std::vector<int*> slots;
    for (int i = 1; i <= 4; i++) {
        int* slot = pool.allocateSlot();
        if (slot != nullptr) {
            *slot = i * 10;
            slots.push_back(slot);
        }
    }

    pool.giveBack(slots[0]);
    pool.giveBack(slots[3]);

    for (int i = 1; i <= 3; i++) {
        int* slot = pool.allocateSlot();
        if (slot != nullptr) {
            std::cout << "Request " << i << ": got slot " << (slot - pool.block) << std::endl;
        } else {
            std::cout << "Request " << i << ": refused" << std::endl;
        }
    }
    return 0;
}
