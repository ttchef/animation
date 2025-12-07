
#include "utils.h"
#include <renderer/shapes.h>
#include <renderer/renderer.h>
#include <containers/darray.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> 

#include <stdio.h> 

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
    // Pos                  // Tex Coords
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,     0.5f, 1.0f,
};

u32 triangle_indices[] = {
    0, 1, 2
};

f32 rectangle_vertices[] = {
     0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
}

void renderer_setup_basic_shapes(Renderer *renderer) {
    for (i32 i = 0; i < SHAPE_COUNT; i++) {
        darrayPush(renderer->VAOS, (gl_u32){0});
        darrayPush(renderer->VBOS, (gl_u32){0});
        darrayPush(renderer->EBOS, (gl_u32){0});
        renderer_setup_shape(renderer, i);
    }
}

void renderer_draw_shape_opengl(Renderer *renderer, usize index, const Shape* shape) {
    gl_u32 program = shape->texture == -1 ? renderer->programs[DEFAULT_PROGRAM] : renderer->programs[TEXTURE_PROGRAM];
    glUseProgram(program);
    gl_u32 colorLoc = glGetUniformLocation(program, "uColor");
    glUniform4f(colorLoc, shape->color.r, shape->color.g, shape->color.b, shape->color.a);
    glBindVertexArray(renderer->VAOS[index]);
    glDrawElements(GL_TRIANGLES, vertexList[index].indiciesSize / sizeof(u32), GL_UNSIGNED_INT, 0);
}

void renderer_draw_shape(Renderer* renderer, const Shape* shape) {
    switch(shape->type) {
        case SHAPE_TYPE_TRIANLGE:
            renderer_draw_shape_opengl(renderer, TRIANGLE_INDEX, shape);
            break;
        case SHAPE_TYPE_RECTANGLE:
            renderer_draw_shape_opengl(renderer, RECTANGLE_INDEX, shape);
            break;
    }
}

TextureID renderer_load_texture(const char *path) {
    gl_u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    i32 width, height, channels;
    u8* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return -1;
    }

    stbi_image_free(data);

    return (TextureID)texture;
}

