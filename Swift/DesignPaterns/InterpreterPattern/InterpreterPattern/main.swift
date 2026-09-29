//
//  main.swift
//  InterpreterPattern
//
//  Created by Anussha on 24/09/26.
//

import Foundation

// The common interface every expression node conforms to
protocol Experssion{
    func evaluate() -> Int
}

// Leaf node: just wraps a literal number, evaluate() returns it directly
struct NumberExpression : Experssion{
    let value : Int
    
    func evaluate() -> Int {
        return value
    }
}

// Composite node: holds two sub-expressions, adds their evaluated results
struct AddExperssion : Experssion{
    let left : Experssion
    let right : Experssion
    
    func evaluate() -> Int {
        return left.evaluate() + right.evaluate()
    }
}

// Composite node: holds two sub-expressions, subtracts their evaluated results
struct SubtractExperssion : Experssion{
    let left : Experssion
    let right : Experssion
    
    func evaluate() -> Int {
        return left.evaluate() - right.evaluate()
    }
}

// Build the tree for: (5 + 3) - 2
let five = NumberExpression(value: 5)
let three = NumberExpression(value: 3)
let two = NumberExpression(value: 2)

let addExpr = AddExperssion(left: five, right: three)        // 5 + 3
let subtractExpr = SubtractExperssion(left: addExpr, right: two) // (5+3) - 2

print("Result: \(subtractExpr.evaluate())")
