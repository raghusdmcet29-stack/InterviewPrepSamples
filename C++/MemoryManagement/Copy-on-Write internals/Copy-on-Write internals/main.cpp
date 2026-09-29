//
//  main.cpp
//  Copy-on-Write internals
//
//  Created by Anussha on 29/09/26.
//

#include <iostream>
#include <vector>

int main() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = a;

    std::cout << "a: " << a.data() << std::endl;
    std::cout << "b: " << b.data() << std::endl;
    b.push_back(4);

    std::cout << "a size: " << a.size() << std::endl;
    std::cout << "b size: " << b.size() << std::endl;
    std::vector<int> e = {1, 2, 3};
    for (int n = 4; n <= 13; n++) {
        e.push_back(n);
        std::cout << "count: " << e.size() << " capacity: " << e.capacity() << std::endl;
    }
    
    std::vector<int> f = {1, 2, 3};
    f.reserve(10);
    std::vector<int> g = f;
    std::cout << "f: " << f.data() << std::endl;
    std::cout << "g: " << g.data() << std::endl;
    std::cout<<"After push g\n";
    g.push_back(4);
    std::cout << "f: " << f.data() << std::endl;
    std::cout << "g: " << g.data() << std::endl;
    std::cout << "f capacity: " << f.capacity() << std::endl;
    std::cout << "g capacity: " << g.capacity() << std::endl;
   
    return 0;
}
