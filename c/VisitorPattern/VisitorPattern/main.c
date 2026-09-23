//
//  main.c
//  VisitorPattern
//
//  Created by Anussha on 23/09/26.
//

#include <stdio.h>

typedef struct Circle Circle;
typedef struct Square Square;

typedef struct {
    void (*visitCircle)(const Circle* circle);
    void (*visitSquare)(const Square* square);
} ShapeVisitor;

typedef struct Shape {
    void (*accept)(const struct Shape* self, const ShapeVisitor* visitor);
} Shape;

struct Circle {
    Shape base;
    double radius;
};

struct Square {
    Shape base;
    double side;
};

void circleAccept(const Shape* self, const ShapeVisitor* visitor) {
    const Circle* circle = (const Circle*)self;
    visitor->visitCircle(circle);
}

void squareAccept(const Shape* self, const ShapeVisitor* visitor) {
    const Square* square = (const Square*)self;
    visitor->visitSquare(square);
}

Circle makeCircle(double radius) {
    Circle c;
    c.base.accept = circleAccept;
    c.radius = radius;
    return c;
}

Square makeSquare(double side) {
    Square s;
    s.base.accept = squareAccept;
    s.side = side;
    return s;
}

void areaVisitCircle(const Circle* circle) {
    double area = 3.14159265358979 * circle->radius * circle->radius;
    printf("Circle area: %f\n", area);
}

void areaVisitSquare(const Square* square) {
    double area = square->side * square->side;
    printf("Square area: %f\n", area);
}

void perimeterVisitCircle(const Circle* circle) {
    double perimeter = 2 * 3.14159265358979 * circle->radius;
    printf("Circle perimeter: %f\n", perimeter);
}

void perimeterVisitSquare(const Square* square) {
    double perimeter = 4 * square->side;
    printf("Square perimeter: %f\n", perimeter);
}

int main(void) {
    Circle circle = makeCircle(5.0);
    Square square = makeSquare(4.0);

    ShapeVisitor areaVisitor;
    areaVisitor.visitCircle = areaVisitCircle;
    areaVisitor.visitSquare = areaVisitSquare;

    circle.base.accept((const Shape*)&circle, &areaVisitor);
    square.base.accept((const Shape*)&square, &areaVisitor);

    ShapeVisitor perimeterVisitor;
        perimeterVisitor.visitCircle = perimeterVisitCircle;
        perimeterVisitor.visitSquare = perimeterVisitSquare;

        circle.base.accept((const Shape*)&circle, &perimeterVisitor);
        square.base.accept((const Shape*)&square, &perimeterVisitor);
    return 0;
}
