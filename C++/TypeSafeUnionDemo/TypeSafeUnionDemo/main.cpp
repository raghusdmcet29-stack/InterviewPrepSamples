//
//  main.cpp
//  TypeSafeUnionDemo
//
//  Created by Anussha on 27/08/26.
//

#include <variant>
#include <iostream>
#include <string>

struct Printer {
    void operator()(int i)const{
        std::cout << "int: " << i << std::endl;
    }
    void operator()(double d) const {
        std::cout << "double: " << d << std::endl;
    }
    void operator()(const std::string& s) const {
        std::cout << "string: " << s << std::endl;
    }
};

int main(){
    std::variant<int,double,std::string> value;
    
    value = 42;
   // std::cout << std::get<int>(value) << std::endl;
    std::visit(Printer{}, value);
    
    value = 3.14;
    std::visit(Printer{}, value);
    
    value = std::string("hello");
    //std::cout << std::get<std::string>(value) << std::endl;
    std::visit(Printer{}, value);
    return 0;
}
