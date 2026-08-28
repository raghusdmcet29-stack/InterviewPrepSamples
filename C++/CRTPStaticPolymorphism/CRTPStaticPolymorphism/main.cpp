//
//  main.cpp
//  CRTPStaticPolymorphism
//
//  Created by Anussha on 28/08/26.
//

#include <iostream>

template<typename Derived>
class Shape{
public:
    double area() const{
        // casts 'this' down to the derived type, then calls its area()
        return static_cast<const Derived*>(this)->areaImpl();
    }
};

class Circle : public Shape<Circle>{
public:
    Circle(double r) : radius(r) {}
    double areaImpl()const{
        return 3.14159 * radius * radius;
    }
private:
    double radius;
    
};

class Square : public Shape<Square>{
public:
    Square(double s): side(s) {}
    double areaImpl()const{
        return side * side;
    }
private:
    double side;
};

class Triangle : public Shape<Triangle> {
public:
    Triangle(double b, double h) : base(b), height(h) {}
    // oops — forgot to add areaImpl()
private:
    double base, height;
};

class BadShape : public Shape<BadShape> {
public:
    double areaImpl() const {
        return area(); // bug: should compute directly, accidentally calls area() again
    }
};

int main(){
    Circle c(2.0);
    Square s (3.0);
    std::cout << "Circle area: " << c.area() << std::endl;
    std::cout << "Square area: " << s.area() << std::endl;
    Triangle t(4.0, 5.0);
   // std::cout << "Triangle area: " << t.area() << std::endl;  // if you enable then only build will fail
    BadShape b;
    //std::cout << "BadShape area: " << b.area() << std::endl; // it will hang if we enable
    return 0;
}
