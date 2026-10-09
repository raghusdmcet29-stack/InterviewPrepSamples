//
//  main.cpp
//  UndefinedBehavior5-InvalidPointers
//
//  Created by Anussha on 09/10/26.
//
#include <iostream>

int main() {
    int *p = nullptr;
    std::cout << *p << "\n";
    return 0;
}
