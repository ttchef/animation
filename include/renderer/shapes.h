
#ifndef SHAPES_H
#define SHAPES_H

#include <utils.h> 

struct Renderer;

typedef i32 TextureID;

typedef enum ShapeType {
    SHAPE_TYPE_TRIANLGE,
    SHAPE_TYPE_RECTANGLE,
} ShapeType;

typedef struct Shape {
    ShapeType type;
    HMM_Vec3 pos;
    Color color;
    TextureID texture; // -1 for no texture
} Shape;

void renderer_setup_basic_shapes(struct Renderer* renderer);
void renderer_draw_shape(struct Renderer* renderer, const Shape* shape);

#endif
