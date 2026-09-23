//
//  main.cpp
//  VisitorPattern
//
//  Created by Anussha on 23/09/26.
//

#include <iostream>

class Circle;
class Square;

class ShapeVisitor {
public:
    virtual void visitCircle(const Circle& circle) = 0;
    virtual void visitSquare(const Square& square) = 0;
    virtual ~ShapeVisitor() = default;
};

class Shape {
public:
    virtual void accept(ShapeVisitor& visitor) const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    double radius;
    Circle(double r) : radius(r) {}
    void accept(ShapeVisitor& visitor) const override {
        visitor.visitCircle(*this);
    }
};

class Square : public Shape {
public:
    double side;
    Square(double s) : side(s) {}
    void accept(ShapeVisitor& visitor) const override {
        visitor.visitSquare(*this);
    }
};

class AreaVisitor : public ShapeVisitor {
public:
    void visitCircle(const Circle& circle) override {
        double area = 3.14159265358979 * circle.radius * circle.radius;
        std::cout << "Circle area: " << area << std::endl;
    }
    void visitSquare(const Square& square) override {
        double area = square.side * square.side;
        std::cout << "Square area: " << area << std::endl;
    }
};

class PerimeterVisitor : public ShapeVisitor {
public:
    void visitCircle(const Circle& circle) override {
        double perimeter = 2 * 3.14159265358979 * circle.radius;
        std::cout << "Circle perimeter: " << perimeter << std::endl;
    }
    void visitSquare(const Square& square) override {
        double perimeter = 4 * square.side;
        std::cout << "Square perimeter: " << perimeter << std::endl;
    }
};
/*
int main() {
    Circle circle(5.0);
    Square square(4.0);

    AreaVisitor areaVisitor;

    circle.accept(areaVisitor);
    square.accept(areaVisitor);

    return 0;
}
*/

#include <vector>

int main() {
    Circle circle(5.0);
    Square square(4.0);

    std::vector<Shape*> shapes = { &circle, &square };

    AreaVisitor areaVisitor;

    for (Shape* shape : shapes) {
        shape->accept(areaVisitor);
    }
    
    PerimeterVisitor perimeterVisitor;
      for (Shape* shape : shapes) {
          shape->accept(perimeterVisitor);
      }

    return 0;
}
