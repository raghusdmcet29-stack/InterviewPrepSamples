//
//  main.swift
//  BridgePattern
//
//  Created by Anussha on 16/09/26.
//

import Foundation

protocol Renderer {
    func renderCircle(radius: Double)
    func renderSquare(side: Double)
}

class RasterRenderer: Renderer {
    func renderCircle(radius: Double) {
        print("Drawing circle as pixels, radius \(radius)")
    }
    func renderSquare(side: Double) {
        print("Drawing square as pixels, side \(side)")
    }
}

class VectorRenderer: Renderer {
    func renderCircle(radius: Double) {
        print("Drawing circle as vector path, radius \(radius)")
    }
    func renderSquare(side: Double) {
        print("Drawing square as vector path, side \(side)")
    }
}

class Shape {
    let renderer: Renderer  // borrowed reference, not owned exclusively

    init(renderer: Renderer) {
        self.renderer = renderer
    }

    func draw() {
        fatalError("Subclasses must override draw()")
    }
}

class Circle: Shape {
    let radius: Double

    init(renderer: Renderer, radius: Double) {
        self.radius = radius
        super.init(renderer: renderer)
    }

    override func draw() {
        renderer.renderCircle(radius: radius)
    }
}

class Square: Shape {
    let side: Double

    init(renderer: Renderer, side: Double) {
        self.side = side
        super.init(renderer: renderer)
    }

    override func draw() {
        renderer.renderSquare(side: side)
    }
}

let raster = RasterRenderer()
let vector = VectorRenderer()

let circle1 = Circle(renderer: raster, radius: 5.0)
let circle2 = Circle(renderer: vector, radius: 5.0)
let square1 = Square(renderer: raster, side: 3.0)

circle1.draw()
circle2.draw()
square1.draw()

