
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h> 
#include <stdbool.h>

#include <glad/glad.h>

#include <renderer/shapes.h> 

typedef struct RendererContext {
    int32_t windowWidth;
    int32_t windowHeight;

    // OpenGL
    uint32_t VAO;
    uint32_t* VBOS;
    uint32_t shaderProgram;
} RendererContext;

bool renderer_init(RendererContext* rendererContext);


#endif
