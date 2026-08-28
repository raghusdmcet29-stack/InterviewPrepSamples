//
//  main.swift
//  TypeErasedShape
//
//  Created by Anussha on 28/08/26.
//

import Foundation

protocol Shape {
    var area: Double { get }
    func intersects(with other : Self)->Bool
}

struct Circle : Shape {
    var radius : Double
    var area: Double {.pi * radius * radius}
    func intersects(with other: Circle) -> Bool {
        false  // placeholder
    }
}

struct Square : Shape {
    var side : Double
    var area: Double { side * side }
    func intersects(with other: Square) -> Bool {
        false  // placeholder
    }
}

struct AnyShape : Shape {
    private let _area : () ->Double
    private let _intersects : (Any) -> Bool
    private let base : Any
    
    init<S : Shape>(_ shape : S){
        base = shape
        _area = {shape.area}
        _intersects = { other in
            guard let other = other as? S else {return false}
            return shape.intersects(with: other)
        }
    }
    var area: Double { _area() }
    func intersects(with other: AnyShape) -> Bool {
         _intersects(other.base)
        }
}

var shapes: [AnyShape] = [AnyShape(Circle(radius: 2)),AnyShape(Square(side: 3))]
for shape in shapes {
    print(shape.area)  //this work fine
}

for shape in shapes {
    print(shape.intersects(with: shape))
}
let c = AnyShape(Circle(radius: 2))
let s = AnyShape(Square(side: 3))
print(c.intersects(with: s)) // should print false safely
print(c.intersects(with: c)) // calls the real Circle.intersects
