
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h> 
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>

#include <renderer/shapes.h> 

typedef struct Renderer {
    usize windowWidth;
    usize windowHeight;

    // OpenGL
    gl_u32* VAOS;
    gl_u32* VBOS;
    gl_u32* EBOS;
    uint32_t shaderProgram;
} Renderer;

bool renderer_init(Renderer* renderer, GLADloadproc loader);
void renderer_resize(Renderer* renderer, size_t width, size_t height);

#endif
