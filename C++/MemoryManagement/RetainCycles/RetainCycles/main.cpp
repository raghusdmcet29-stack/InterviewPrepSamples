//
//  main.cpp
//  RetainCycles
//
//  Created by Anussha on 28/09/26.
//

#include <iostream>
#include <memory>
#include <string>

/*class Apartment; // forward declaration

class Person {
public:
    std::string name;
    std::shared_ptr<Apartment> apartment;

    Person(std::string n) : name(n) {
        std::cout << "Person " << name << " created\n";
    }
    ~Person() {
        std::cout << "Person " << name << " freed\n";
    }
};

class Apartment {
public:
    std::string unit;
    std::weak_ptr<Person> tenant;  // to break retain cycle we used weak_ptr
//  std::shared_ptr<Person> tenant; // if you user shared_ptr it will cause retain cycle
    Apartment(std::string u) : unit(u) {
        std::cout << "Apartment " << unit << " created\n";
    }
    ~Apartment() {
        std::cout << "Apartment " << unit << " freed\n";
    }
};*/

class CreditCard;

class Customer {
public:
    std::string name;
    std::shared_ptr<CreditCard> card;

    Customer(std::string n) : name(n) {
        std::cout << "Customer " << name << " created\n";
    }
    ~Customer() {
        std::cout << "Customer " << name << " freed\n";
    }
};

class CreditCard {
public:
    std::string number;
    Customer* owner;

    CreditCard(std::string num, Customer* o) : number(num), owner(o) {
        std::cout << "Card " << number << " created\n";
    }
    ~CreditCard() {
        std::cout << "Card " << number << " freed\n";
    }
};

int main() {
    
    auto dave = std::make_shared<Customer>("Dave");
    auto savedCard = std::make_shared<CreditCard>("5678", dave.get());
    dave->card = savedCard;

    dave.reset();
    std::cout << "About to read the owner\n";
    std::cout << savedCard->owner->name << "\n";
   /* auto alice = std::make_shared<Person>("Alice");
    auto flat = std::make_shared<Apartment>("4B");

    alice->apartment = flat;
    flat->tenant = alice;

    alice.reset();
    flat.reset();
   */
    
    //Reading through a weak_ptr.
    
    /*auto carol = std::make_shared<Person>("Carol");
    auto flat2 = std::make_shared<Apartment>("7C");
    flat2->tenant = carol;

    if (auto t = flat2->tenant.lock()) {
        std::cout << "Tenant is " << t->name << "\n";
    }

    carol.reset();

    if (auto t = flat2->tenant.lock()) {
        std::cout << "Tenant is " << t->name << "\n";
    } else {
        std::cout << "Tenant is gone\n";
    }
    std::cout << "End of program\n";*/
}
