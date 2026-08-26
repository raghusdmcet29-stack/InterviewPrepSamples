//
//  main.cpp
//  MiniSharedPtr
//
//  Created by Anussha on 26/08/26.
//

#include <iostream>

template<typename T>
class MiniSharedPtr {
private:
    T* ptr;
    int* refCount;
    
public:
    explicit MiniSharedPtr(T* p = nullptr) : ptr(p),refCount(new int(p ? 1 : 0)){
        std::cout << "Constructed, refCount = " << *refCount << std::endl;
    }
    // Copy constructor - share ownership, increment count
        MiniSharedPtr(const MiniSharedPtr& other) : ptr(other.ptr), refCount(other.refCount) {
            if (refCount) {
                (*refCount)++;
            }
            std::cout << "Copy-constructed, refCount = " << *refCount << std::endl;
        }
    
    // Copy assignment - release old resource, share new one
       MiniSharedPtr& operator=(const MiniSharedPtr& other) {
           if (this == &other) {
               return *this; // self-assignment guard
           }
           release(); // give up our current ownership first

           ptr = other.ptr;
           refCount = other.refCount;
           if (refCount) {
               (*refCount)++;
           }
           std::cout << "Copy-assigned, refCount = " << *refCount << std::endl;
           return *this;
       }
    
    // Move constructor - steal ownership, no refCount increment
        MiniSharedPtr(MiniSharedPtr&& other) noexcept
            : ptr(other.ptr), refCount(other.refCount) {
            other.ptr = nullptr;
            other.refCount = nullptr;
            std::cout << "Move-constructed" << std::endl;
        }
      
    // Move assignment - release old resource, steal new one
        MiniSharedPtr& operator=(MiniSharedPtr&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            release(); // give up whatever we currently own

            ptr = other.ptr;
            refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;

            std::cout << "Move-assigned" << std::endl;
            return *this;
        }
    ~MiniSharedPtr() {
        release();
    }
    
private:
    void release(){
        if (refCount) {
            (*refCount)--;
            std::cout << "Released, refCount = " << *refCount << std::endl;
            if (*refCount == 0) {
                delete ptr;
                delete refCount;
                std::cout << "Resource freed" << std::endl;
            }
        }
    }
};

/*int main() {
    {
        MiniSharedPtr<int> p(new int(42));
        std::cout << "Inside scope" << std::endl;
    } // p goes out of scope here -> destructor runs

    std::cout << "After scope" << std::endl;
    return 0;
}
*/
/*
int main() {
    MiniSharedPtr<int> a(new int(42));
    {
        MiniSharedPtr<int> b = a;   // shallow copy - no refCount++ happens!
        std::cout << "Inside inner scope" << std::endl;
    } // b's destructor runs here

    std::cout << "After inner scope" << std::endl;
    return 0;
} // a's destructor runs here -> CRASH (double free)
*/
int main() {
    MiniSharedPtr<int> a(new int(42));
    std::cout << "--- moving a into c ---" << std::endl;
    MiniSharedPtr<int> c = std::move(a);   // move constructor, not copy
    std::cout << "--- end of main ---" << std::endl;
    return 0;
}
