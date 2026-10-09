//
//  main.cpp
//  MoveSemantics3-MoveAssignment
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
    Box(Box&& other) noexcept : value(other.value) {
        other.value = nullptr;
        std::cout << "moved\n";
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
private:
    int* value;
};

int main() {
    Box a(10);
    Box b = std::move(a);
    Box c(20);
    c = std::move(b);
    c.print();
    b.print();
    return 0;
}
