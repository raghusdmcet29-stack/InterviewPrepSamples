//
//  main.cpp
//  DanglingPointers
//
//  Created by Anussha on 01/10/26.
//
// fix for the dangling pointer for P
#include <iostream>

int main() {
    int *p = new int(42);
    int *q = p;

    std::cout << "Before free: " << *p << std::endl;

    delete p;
    p = nullptr; // no dangling

    std::cout << "Freed." << std::endl;

    if (p != nullptr) {
        std::cout << "p after free: " << *p << std::endl;
    } else {
        std::cout << "p is null, read skipped" << std::endl;
    }

    if (q != nullptr) { // still dangling becasue not set nullptr
        std::cout << "q after free: " << *q << std::endl;
    } else {
        std::cout << "q is null, read skipped" << std::endl;
    }

    return 0;
}


// which create dangling pointer
/*
#include <iostream>

int main() {
    int *p = new int(42);
    int *q = p;
    std::cout << "Before free: " << *p << std::endl;

    delete p;
    p = nullptr;

    std::cout << "Freed." << std::endl;
    std::cout << "After free: " << *q << std::endl;
    if (p != nullptr) {
        std::cout << "After free: " << *p << std::endl;
    } else {
        std::cout << "Pointer is null, read skipped" << std::endl;
    }

    return 0;
}
*/
