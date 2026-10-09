//
//  main.cpp
//  MoveSemantics2-MoveConstructor
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
    b.print();
    a.print();
    return 0;
}


/*
#include <iostream>

class Box {
public:
    Box(int v) : value(new int(v)) {
        std::cout << "constructed\n";
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
    a.print();
    return 0;
}
*/
