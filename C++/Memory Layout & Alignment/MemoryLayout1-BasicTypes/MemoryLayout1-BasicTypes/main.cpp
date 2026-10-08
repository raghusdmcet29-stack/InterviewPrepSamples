//
//  main.cpp
//  MemoryLayout1-BasicTypes
//
//  Created by Anussha on 08/10/26.
//

#include <iostream>
#include <cstdint>

int main() {
    std::cout << "int8_t  size: " << sizeof(int8_t)  << " alignment: " << alignof(int8_t)  << "\n";
    std::cout << "int32_t size: " << sizeof(int32_t) << " alignment: " << alignof(int32_t) << "\n";
    std::cout << "int64_t size: " << sizeof(int64_t) << " alignment: " << alignof(int64_t) << "\n";
    std::cout << "bool    size: " << sizeof(bool)    << " alignment: " << alignof(bool)    << "\n";
    std::cout << "double  size: " << sizeof(double)  << " alignment: " << alignof(double)  << "\n";
    return 0;
}
