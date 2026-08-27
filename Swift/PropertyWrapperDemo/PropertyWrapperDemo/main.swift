//
//  main.swift
//  PropertyWrapperDemo
//
//  Created by Anussha on 27/08/26.
//

import Foundation

@propertyWrapper
struct Clamped<Value: Comparable> {
    private var value: Value
    private let range: ClosedRange<Value>

    init(wrappedValue: Value, _ range: ClosedRange<Value>) {
        self.range = range
        self.value = min(max(wrappedValue, range.lowerBound), range.upperBound)
    }

    var wrappedValue: Value {
        get { value }
        set { value = min(max(newValue, range.lowerBound), range.upperBound) }
    }
}

struct GameCharacter {
    @Clamped(0...100) var health : Int = 100
    @Clamped(0...100) var mana : Int = 50
}

var hero = GameCharacter()
print("Initial health:", hero.health)   // 100
print("Initial mana:", hero.mana)       // 50

hero.health -= 150   // try to drive it negative
print("After big damage, health:", hero.health)   // should clamp to 0, not -50

hero.mana += 9999    // try to overflow it
print("After mana potion overload:", hero.mana)   // should clamp to 100, not 10049

var weakling = GameCharacter(health: -20)  // init with out-of-range value
print("Weakling initial health:", weakling.health) // should clamp to 0 at init, not -20
