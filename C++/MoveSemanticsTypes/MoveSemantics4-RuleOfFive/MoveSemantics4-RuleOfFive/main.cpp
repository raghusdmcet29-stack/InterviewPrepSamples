//
//  main.cpp
//  MoveSemantics4-RuleOfFive
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>
#include <utility>

class Box {
public:
    Box(int v) : value(new int(v)) {
        std::cout << "constructed\n";
    }
    Box(const Box& other) : value(new int(*other.value)) {
        std::cout << "copied\n";
    }
    Box(Box&& other) noexcept : value(other.value) {
        other.value = nullptr;
        std::cout << "moved\n";
    }
    Box& operator=(const Box& other) {
        if (this != &other) {
            delete value;
            value = new int(*other.value);
        }
        std::cout << "copy assigned\n";
        return *this;
    }
    Box& operator=(Box&& other) noexcept {
        if (this != &other) {
            delete value;
            value = other.value;
            other.value = nullptr;
        }
        std::cout << "move assigned\n";
        return *this;
    }
    ~Box() {
        delete value;
    }
    void print() const {
        if (value) std::cout << "value = " << *value << "\n";
        else std::cout << "empty\n";
    }
    void set(int v) {
        *value = v;
    }
private:
    int* value;
};

int main() {
    Box a(10);
    Box b(20);
    b = a;
    a.print();
    b.print();
    return 0;
}
/*
int main() {
    Box a(10);
    Box b = a;
    b.set(99);
    a.print();
    b.print();
    return 0;
}
*/
