//
//  main.cpp
//  UndefinedBehavior4-StrictAliasing
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>
#include <cstring>

int main() {
    float f = 1.5f;
    int bits;
    std::memcpy(&bits, &f, sizeof(bits));
    std::cout << bits << "\n";
    return 0;
}
/*
#include <iostream>

int main() {
    float f = 1.5f;
    int *p = reinterpret_cast<int *>(&f);
    std::cout << *p << "\n";
    return 0;
}
*/
