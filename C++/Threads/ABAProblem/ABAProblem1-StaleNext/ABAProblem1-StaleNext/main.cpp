//
//  main.cpp
//  ABAProblem1-StaleNext
//
//  Created by Anussha on 14/10/26.
//
//part 2

#include <iostream>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

struct Stack {
    Node* top = nullptr;

    // Simulated CAS: swap top only if it still equals expected
    bool compareAndSwapTop(Node* expected, Node* newTop) {
        if (top == expected) {
            top = newTop;
            return true;
        }
        return false;
    }
};

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);
    Stack stack;
    stack.top = a;

    // Thread 1 reads the top and its next, then pauses
    Node* expectedTop = stack.top;   // A
    Node* savedNext = expectedTop->next;  // B

    // Thread 2 pops A, pops B, pushes A back
    stack.top = b;
    stack.top = nullptr;
    a->next = nullptr;
    stack.top = a;

    // Thread 1 resumes and tries its CAS
    bool succeeded = stack.compareAndSwapTop(expectedTop, savedNext);
    std::cout << "CAS succeeded: " << (succeeded ? "true" : "false") << "\n";
    std::cout << "top value: " << stack.top->value << "\n";

    delete a;
    delete b;
    return 0;
}




//part 1
/*
#include <iostream>

struct Node {
    int value;
    Node* next;
    Node(int v, Node* n = nullptr) : value(v), next(n) {}
};

int main() {
    Node* b = new Node(2);
    Node* a = new Node(1, b);

    std::cout << "a value: " << a->value << "\n";
    std::cout << "a next value: " << a->next->value << "\n";
    std::cout << "a == b? " << (a == b) << "\n";

    delete a;
    delete b;
    return 0;
}
*/
