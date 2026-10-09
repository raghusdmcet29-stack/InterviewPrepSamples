//
//  main.cpp
//  vtablepointer
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>

struct Plain {
    int x;
};

struct WithVirtual {
    int x;
    virtual void speak() {}
};

int main() {
    std::cout << "Plain: sizeof " << sizeof(Plain) << "\n";
    std::cout << "WithVirtual: sizeof " << sizeof(WithVirtual) << "\n";
    return 0;
}
