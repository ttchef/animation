
#ifndef SHAPES_H
#define SHAPES_H

#include <renderer/types.h>

struct RendererContext;

extern float triangle_vertices[9];

void renderer_setup_basic_shapes(struct RendererContext* renderContext);
void renderer_draw_triangle(struct RendererContext* renderContext, Color color);

#endif
