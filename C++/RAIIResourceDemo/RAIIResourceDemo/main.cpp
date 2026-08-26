//
//  main.cpp
//  RAIIResourceDemo
//
//  Created by Anussha on 25/08/26.
//

#include <iostream>

class ResourceWrapper{
private:
    int* data;
public:
    ResourceWrapper(int value){
        data = new int(value);
        std::cout<<"Constructed, Value="<<*data<<std::endl;
    }
    ResourceWrapper(const ResourceWrapper&) = delete;
    ResourceWrapper& operator=(const ResourceWrapper&) = delete;
    // Move constructor
    ResourceWrapper(ResourceWrapper&& other) noexcept {
        data = other.data;
        other.data = nullptr;
        std::cout << "Move constructed" << std::endl;
    }

    // Move assignment
    ResourceWrapper& operator=(ResourceWrapper&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
            std::cout << "Move assigned" << std::endl;
        }
        return *this;
    }
    ~ResourceWrapper(){
        if (data) {
            std::cout<<"Destroyed, value ="<<*data<<std::endl;
        }else {
            std::cout<<"Destroyed,but data was null (Moved_from)"<<std::endl;
        }
        delete data;
    }
};

int main() {
    std::cout << "Before scope" << std::endl;
    {
        ResourceWrapper r(42);
        std::cout << "Inside scope" << std::endl;
    }
    std::cout << "After scope" << std::endl;
    
    ResourceWrapper r1(42);
    //ResourceWrapper r2 = r1;  // default copy constructor — shallow copy
    ResourceWrapper r2 = std::move(r1);
    return 0;
}
