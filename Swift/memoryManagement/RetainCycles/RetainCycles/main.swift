//
//  main.swift
//  RetainCycles
//
//  Created by Anussha on 28/09/26.
//

import Foundation


// using unowned to break retail cycle (The difference is that it is never nil and never optional)

class Customer {
    let name : String
    var card : CreditCard?
    
    init(name: String) {
        self.name = name
        print("Customer \(name) created")
    }
    
    deinit {
        print("Customer \(name) freed")
    }
}

class CreditCard{
    let number : String
    unowned let owner : Customer
    
    init(number: String, owner: Customer) {
        self.number = number
        self.owner = owner
        print("Card \(number) created")
    }
    deinit{
        print("Card \(number) freed")
    }
}
/*//What happens when unowned is wrong.

var dave: Customer? = Customer(name: "Dave")
let savedCard = CreditCard(number: "5678", owner: dave!)
dave?.card = savedCard

dave = nil
print("About to read the owner")
print(savedCard.owner.name)
*/
var bob: Customer? = Customer(name: "Bob")
bob?.card = CreditCard(number: "1234", owner: bob!)

bob = nil
print("End of customer example")
 


// Using weak to break retain cycle example
/*class Person {
    let name : String
    var apartment : Apartment?
    
    init(name: String) {
        self.name = name
        print("Person \(name) created")
    }
    
    deinit{
        print("Person \(name) freed")
    }
}

class Apartment{
    let unit : String
   weak var tenant : Person?
    
    init(unit: String){
        self.unit = unit
        print("Apartment \(unit) created")
    }
    
    deinit{
        print("Apartment \(unit) freed")
    }
}

var alice: Person? = Person(name: "Alice")
var flat: Apartment? = Apartment(unit: "4B")

alice?.apartment = flat
flat?.tenant = alice
alice = nil
flat = nil



print("End of program")*/
