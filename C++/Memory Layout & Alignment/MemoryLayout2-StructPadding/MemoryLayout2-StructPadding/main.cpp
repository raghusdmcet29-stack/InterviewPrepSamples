//
//  main.cpp
//  MemoryLayout2-StructPadding
//
//  Created by Anussha on 08/10/26.
//

#include <iostream>
#include <cstdint>

struct Mixed {
    int8_t a;
    int32_t b;
    int8_t c;
};

struct Reordered {
    int32_t b;
    int8_t a;
    int8_t c;
};

int main() {
    std::cout << "Mixed     sizeof: " << sizeof(Mixed)     << " alignof: " << alignof(Mixed)     << "\n";
    std::cout << "Reordered sizeof: " << sizeof(Reordered) << " alignof: " << alignof(Reordered) << "\n";
    return 0;
}
