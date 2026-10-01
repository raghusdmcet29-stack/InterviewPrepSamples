//
//  main.cpp
//  StackVsHeap
//
//  Created by Anussha on 01/10/26.
//


#include <iostream>
#include <memory>

class Box {
public:
    int value = 1;
    ~Box() { std::cout << "Box freed" << std::endl; }
};

std::unique_ptr<Box> makeBox() {
    std::unique_ptr<Box> box = std::make_unique<Box>();
    std::cout << "inside makeBox:  " << box.get() << std::endl;
    std::cout << "makeBox ending" << std::endl;
    return box;
}

void caller() {
    std::unique_ptr<Box> b = makeBox();
    std::cout << "outside makeBox: " << b.get() << std::endl;
    std::cout << "caller ending" << std::endl;
}

int main() {
    caller();
    std::cout << "after caller" << std::endl;
    return 0;
}


/*
#include <iostream>

int makeByValue() {
    int x = 10;
    return x;
}

int main() {
    int v = makeByValue();
    std::cout << "v address: " << &v << std::endl;
    std::cout << "value: " << v << std::endl;
    return 0;
}*/
/*
#include <iostream>

int *makeOnStack() {
    int x = 10;
    return &x;
}

int main() {
    int *p = makeOnStack();
    std::cout << "p points to: " << p << std::endl;
    std::cout << "value: " << *p << std::endl;
    return 0;
}
*/
/*
#include <iostream>

int main() {
    int x = 10;
    int *h = new int(20);

    std::cout << "x address (stack): " << &x << std::endl;
    std::cout << "h points to (heap): " << h << std::endl;

    delete h;
    std::cout << "h itself (address of the variable): " << &h << std::endl;
    return 0;
}
*/
