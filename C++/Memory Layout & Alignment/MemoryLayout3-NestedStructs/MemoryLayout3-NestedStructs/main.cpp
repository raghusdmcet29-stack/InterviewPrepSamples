//
//  main.cpp
//  MemoryLayout3-NestedStructs
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>
#include <cstdint>

struct Inner {
    int8_t a;
    int32_t b;
};

struct Outer {
    int8_t x;
    Inner inner;
};

int main() {
    std::cout << "Inner: sizeof " << sizeof(Inner) << ", alignof " << alignof(Inner) << "\n";
    std::cout << "Outer: sizeof " << sizeof(Outer) << ", alignof " << alignof(Outer) << "\n";
    return 0;
}
