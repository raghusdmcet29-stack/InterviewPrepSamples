//
//  main.cpp
//  BridgePattern
//
//  Created by Anussha on 16/09/26.
//

#include <iostream>


class Renderer {
public:
    virtual void renderCircle(double radius) = 0;
    virtual void renderSquare(double side) = 0;
    virtual ~Renderer() = default;
};

class RasterRenderer : public Renderer {
public:
    void renderCircle(double radius) override {
        std::cout << "Drawing circle as pixels, radius " << radius << "\n";
    }
    void renderSquare(double side) override {
        std::cout << "Drawing square as pixels, side " << side << "\n";
    }
};

class VectorRenderer : public Renderer {
public:
    void renderCircle(double radius) override {
        std::cout << "Drawing circle as vector path, radius " << radius << "\n";
    }
    void renderSquare(double side) override {
        std::cout << "Drawing square as vector path, side " << side << "\n";
    }
};

class Shape {
protected:
    Renderer& renderer;  // borrowed reference — Shape does not own this

public:
    Shape(Renderer& renderer) : renderer(renderer) {}
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double radius;

public:
    Circle(Renderer& renderer, double radius) : Shape(renderer), radius(radius) {}

    void draw() override {
        renderer.renderCircle(radius);
    }
};

class Square : public Shape {
    double side;

public:
    Square(Renderer& renderer, double side) : Shape(renderer), side(side) {}

    void draw() override {
        renderer.renderSquare(side);
    }
};
int main() {
    RasterRenderer raster;
    VectorRenderer vector;

    Circle circle1(raster, 5.0);
    Circle circle2(vector, 5.0);
    Square square1(raster, 3.0);

    circle1.draw();
    circle2.draw();
    square1.draw();

    return 0;
}
