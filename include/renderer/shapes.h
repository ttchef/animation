
#ifndef SHAPES_H
#define SHAPES_H

#include <utils.h> 
#include <firef/firef.h>

struct Renderer;

typedef i32 TextureID;
typedef i32 MeshID;

typedef struct Mesh {
    MeshID id;
    u32 index_count;
} Mesh;

typedef enum ShapeType {
    SHAPE_TYPE_TRIANLGE,
    SHAPE_TYPE_RECTANGLE,
    SHAPE_TYPE_CUSTOM,
} ShapeType;

typedef struct Shape {
    ShapeType type;
    HMM_Vec3 pos;
    Color color;
    TextureID texture; // -1 for no texture (tmp)
    Mesh mesh; // MeshID == -1 for no mesh (tmp)
} Shape;

void renderer_setup_basic_shapes(struct Renderer* renderer);
void renderer_draw_shape(struct Renderer* renderer, const Shape* shape);

#endif
