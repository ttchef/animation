
#include "utils.h"
#include <renderer/shapes.h>
#include <renderer/renderer.h>
#include <containers/darray.h>

enum {
    TRIANGLE_INDEX,
    RECTANGLE_INDEX,
    SHAPE_COUNT
};

typedef struct VertexList {
    f32* vertices;
    usize verticesSize;
    u32* indicies;
    usize indiciesSize;
} VertexList;

f32 triangle_vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

u32 triangle_indices[] = {
    0, 1, 2
};

f32 rectangle_vertices[] = {
     0.5f,  0.5f, 0.0f,  
     0.5f, -0.5f, 0.0f,  
    -0.5f, -0.5f, 0.0f,   
    -0.5f,  0.5f, 0.0f  
};

u32 rectangle_indices[] = {
    0, 1, 3,
    1, 2, 3
};

VertexList vertexList[] = {
    { triangle_vertices, sizeof(triangle_vertices), triangle_indices, sizeof(triangle_indices)},
    { rectangle_vertices, sizeof(rectangle_vertices), rectangle_indices, sizeof(rectangle_indices)},
};

void renderer_setup_shape(Renderer* renderer, u32 index) {
    glGenVertexArrays(1, &renderer->VAOS[index]);
    glBindVertexArray(renderer->VAOS[index]);

    glGenBuffers(1, &renderer->VBOS[index]);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBOS[index]);
    glBufferData(GL_ARRAY_BUFFER, vertexList[index].verticesSize, vertexList[index].vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &renderer->EBOS[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBOS[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexList[index].indiciesSize, vertexList[index].indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void renderer_setup_basic_shapes(Renderer *renderer) {
    for (i32 i = 0; i < SHAPE_COUNT; i++) {
        darrayPush(renderer->VAOS, (gl_u32){0});
        darrayPush(renderer->VBOS, (gl_u32){0});
        darrayPush(renderer->EBOS, (gl_u32){0});
        renderer_setup_shape(renderer, i);
    }
}

void renderer_draw_triangle(Renderer *renderer, usize index, Color color) {
    glUseProgram(renderer->shaderProgram);
    gl_u32 colorLoc = glGetUniformLocation(renderer->shaderProgram, "uColor");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    glBindVertexArray(renderer->VAOS[index]);
    glDrawElements(GL_TRIANGLES, vertexList[index].indiciesSize / sizeof(u32), GL_UNSIGNED_INT, 0);
}

void renderer_draw_shape(Renderer* renderer, const Shape* shape) {
    switch(shape->type) {
        case SHAPE_TYPE_TRIANLGE:
            renderer_draw_triangle(renderer, TRIANGLE_INDEX, shape->color);
            break;
        case SHAPE_TYPE_RECTANGLE:
            renderer_draw_triangle(renderer, RECTANGLE_INDEX, shape->color);
            break;
    }
}

