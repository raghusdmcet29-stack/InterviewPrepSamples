//
//  main.cpp
//  UndefinedBehavior1-SignedOverflow
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>
#include <limits>

int main() {
    int big = std::numeric_limits<int>::max();
    if (big + 1 < big) {
        std::cout << "overflow detected\n";
    } else {
        std::cout << "no overflow detected\n";
    }
    return 0;
}

/*
#include <iostream>
#include <limits>

int main() {
    int big = std::numeric_limits<int>::max();
    big = big + 1;
    std::cout << big << "\n";
    return 0;
}
*/
/*
#include <iostream>
#include <cstdint>

int main() {
    int8_t big = 127;
    big = big + 1;
    std::cout << static_cast<int>(big) << "\n";
    return 0;
}
*/
