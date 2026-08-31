//
//  main.cpp
//  CustomAllocator
//
//  Created by Anussha on 31/08/26.
//
#include <iostream>
#include <vector>
#include <memory>

static std::size_t totalAllocated = 0;

template <typename T>
struct TrackingAllocator {
    using value_type = T;
    
    TrackingAllocator() = default;
    
    // Required for rebind when vector needs a different type internally
    template <typename U>
    TrackingAllocator(const TrackingAllocator<U>&) noexcept {}
    
    T* allocate(std::size_t n){
        std::size_t bytes = n * sizeof(T);
        totalAllocated += bytes;
        std::cout << "[allocate] " << bytes << " bytes | running total: " << totalAllocated << "\n";
        return static_cast<T*>(::operator new(bytes));
    }
    
    void deallocate(T* p, std::size_t n)noexcept{
        std::size_t bytes = n * sizeof(T);
        totalAllocated -= bytes;
        std::cout << "[deallocate] " << bytes << " bytes | running total: " << totalAllocated << "\n";
                ::operator delete(p);
    }
};

// Allocators of the same underlying type are always considered equal here
template <typename T,typename U>
bool operator==(const TrackingAllocator<T>&,const TrackingAllocator<U>&){return true;}

template<typename T,typename U>
bool operator!=(const TrackingAllocator<T>&,const TrackingAllocator<U>&){return false;}

int main(){
    {
        std::vector<int,TrackingAllocator<int>> vec;
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);
        vec.push_back(4);
        vec.push_back(5);
        vec.push_back(6);
        vec.push_back(7);
        vec.push_back(8);
        std::cout << "--- vec about to go out of scope ---\n";
    } // vec destructor runs here, inside this inner scope
    std::cout << "Final totalAllocated (should be 0): " << totalAllocated << "\n";
    return 0;
}
