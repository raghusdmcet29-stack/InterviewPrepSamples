//
//  main.cpp
//  UndefinedBehavior3-OutOfBounds
//
//  Created by Anussha on 09/10/26.
//
#include <iostream>

int main() {
    int numbers[3] = {10, 20, 30};
    std::cout << numbers[3] << "\n";
    return 0;
}
