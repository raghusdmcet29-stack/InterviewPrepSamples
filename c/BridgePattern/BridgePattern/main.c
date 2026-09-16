//
//  main.c
//  BridgePattern
//
//  Created by Anussha on 16/09/26.
//

#include <stdio.h>

typedef struct {
    void (*renderCircle)(double radius);
    void (*renderSquare)(double side);
} Renderer;

void raster_renderCircle(double radius) {
    printf("Drawing circle as pixels, radius %.1f\n", radius);
}
void raster_renderSquare(double side) {
    printf("Drawing square as pixels, side %.1f\n", side);
}

void vector_renderCircle(double radius) {
    printf("Drawing circle as vector path, radius %.1f\n", radius);
}
void vector_renderSquare(double side) {
    printf("Drawing square as vector path, side %.1f\n", side);
}

Renderer make_raster_renderer(void) {
    Renderer r;
    r.renderCircle = raster_renderCircle;
    r.renderSquare = raster_renderSquare;
    return r;
}

Renderer make_vector_renderer(void) {
    Renderer r;
    r.renderCircle = vector_renderCircle;
    r.renderSquare = vector_renderSquare;
    return r;
}

typedef struct {
    double radius;
} CircleData;

typedef struct {
    double side;
} SquareData;

typedef struct Shape {
    Renderer* renderer;      // borrowed pointer — Shape does not own this
    void* shapeData;         // points to CircleData or SquareData
    void (*draw)(struct Shape* self);
} Shape;

void circle_draw(Shape* self) {
    CircleData* data = (CircleData*)self->shapeData;
    self->renderer->renderCircle(data->radius);
}

void square_draw(Shape* self) {
    SquareData* data = (SquareData*)self->shapeData;
    self->renderer->renderSquare(data->side);
}

Shape make_circle(Renderer* renderer, CircleData* data) {
    Shape s;
    s.renderer = renderer;
    s.shapeData = data;
    s.draw = circle_draw;
    return s;
}

Shape make_square(Renderer* renderer, SquareData* data) {
    Shape s;
    s.renderer = renderer;
    s.shapeData = data;
    s.draw = square_draw;
    return s;
}

int main(void) {
    Renderer raster = make_raster_renderer();
    Renderer vector = make_vector_renderer();

    CircleData circle1Data = { .radius = 5.0 };
    CircleData circle2Data = { .radius = 5.0 };
    SquareData square1Data = { .side = 3.0 };

    Shape circle1 = make_circle(&raster, &circle1Data);
    Shape circle2 = make_circle(&vector, &circle2Data);
    Shape square1 = make_square(&raster, &square1Data);

    circle1.draw(&circle1);
    circle2.draw(&circle2);
    square1.draw(&square1);

    return 0;
}
