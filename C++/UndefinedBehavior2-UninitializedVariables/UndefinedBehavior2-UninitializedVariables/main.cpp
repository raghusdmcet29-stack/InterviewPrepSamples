//
//  main.cpp
//  UndefinedBehavior2-UninitializedVariables
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>

int main() {
    int count = 0;
    std::cout << count << "\n";
    return 0;
}

/*
#include <iostream>

int main() {
    int count;
    std::cout << count << "\n";
    return 0;
}
*/
