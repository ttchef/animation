
#ifndef SHAPES_H
#define SHAPES_H

#include <stdint.h>

struct Renderer;

extern float triangle_vertices[9];

void renderer_setup_basic_shapes(struct Renderer* renderer);
void renderer_draw_triangle(struct Renderer* renderer);

#endif
