//
//  main.cpp
//  ShapePolymorphismDemo
//
//  Created by Anussha on 25/08/26.
//

#include <iostream>

class Shape{
public:
    virtual double area() const = 0; // pure virtual function
    virtual ~Shape() {std::cout << "Shape destructor called" << std::endl;} // cirtual destructor
};

class Circle : public Shape{
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    ~Circle() {
            std::cout << "Circle destructor called" << std::endl;
        }
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape{
private:
    double width,height;
public:
    Rectangle(double w,double h) : width(w),height(h) {}
    ~Rectangle() {
            std::cout << "Rectangle destructor called" << std::endl;
        }
    
    double area() const override {
        return width * height;
    }
};

/*int main() {
    Circle c(5.0);
    Rectangle r(4.0, 6.0);

    Shape* shapes[2] = { &c, &r };

    for (int i = 0; i < 2; i++) {
        std::cout << "Area: " << shapes[i]->area() << std::endl;
    }

    return 0;
}
*/

int main() {
    Shape* shape = new Rectangle(4.0, 6.0);
    delete shape;
    return 0;
}
