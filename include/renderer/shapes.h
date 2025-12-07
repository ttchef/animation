
#ifndef SHAPES_H
#define SHAPES_H

#include <utils.h> 

struct Renderer;

typedef enum ShapeType {
    SHAPE_TYPE_TRIANLGE,
    SHAPE_TYPE_RECTANGLE,
} ShapeType;

typedef struct Shape {
    ShapeType type;
    HMM_Vec3 pos;
    Color color;
} Shape;

void renderer_setup_basic_shapes(struct Renderer* renderer);
void renderer_draw_shape(struct Renderer* renderer, const Shape* shape);

#endif
