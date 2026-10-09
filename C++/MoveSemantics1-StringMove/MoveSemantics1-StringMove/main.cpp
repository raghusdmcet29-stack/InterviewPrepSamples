//
//  main.cpp
//  MoveSemantics1-StringMove
//
//  Created by Anussha on 09/10/26.
//

#include <iostream>
#include <string>
#include <utility>

int main() {
    std::string a = "hello";
    std::string b = std::move(a);
    std::cout << "b = " << b << "\n";
    std::cout << "a = '" << a << "'\n";
    a = "new";
    std::cout << "a after reassigning = '" << a << "'\n";
    return 0;
}


/*
#include <iostream>
#include <string>

int main() {
    std::string a = "hello";
    std::string b = a;
    std::cout << "b = " << b << "\n";
    std::cout << "a = '" << a << "'\n";
    b += " world";
    std::cout << "after change, a = '" << a << "'\n";
    a = "new";
    std::cout << "a after reassigning = '" << a << "'\n";
    return 0;
}*/
/*
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::string a = "hello";
    std::string b = std::move(a);
    std::cout << "b = " << b << "\n";
    std::cout << "a = '" << a << "'\n";
    return 0;
}
*/
