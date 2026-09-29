//
//  main.swift
//  VisitorPattern
//
//  Created by Anussha on 23/09/26.
//

import Foundation

protocol ShapeVisitor {
    func visitCircle(_ circle: Circle)
    func visitSquare(_ square: Square)
}

protocol Shape {
    func accept(_ visitor: ShapeVisitor)
}

class Circle: Shape {
    let radius: Double
    init(radius: Double) {
        self.radius = radius
    }
    func accept(_ visitor: ShapeVisitor) {
        visitor.visitCircle(self)
    }
}

class Square: Shape {
    let side: Double
    init(side: Double) {
        self.side = side
    }
    func accept(_ visitor: ShapeVisitor) {
        visitor.visitSquare(self)
    }
}

class AreaVisitor: ShapeVisitor {
    func visitCircle(_ circle: Circle) {
        let area = Double.pi * circle.radius * circle.radius
        print("Circle area: \(area)")
    }
    func visitSquare(_ square: Square) {
        let area = square.side * square.side
        print("Square area: \(area)")
    }
}

class PerimeterVisitor: ShapeVisitor {
    func visitCircle(_ circle: Circle) {
        let perimeter = 2 * Double.pi * circle.radius
        print("Circle perimeter: \(perimeter)")
    }
    func visitSquare(_ square: Square) {
        let perimeter = 4 * square.side
        print("Square perimeter: \(perimeter)")
    }
}

let shapes: [Shape] = [Circle(radius: 5), Square(side: 4)]
let areaVisitor = AreaVisitor()

for shape in shapes {
    shape.accept(areaVisitor)
}

let perimeterVisitor = PerimeterVisitor()

for shape in shapes {
    shape.accept(perimeterVisitor)
}

