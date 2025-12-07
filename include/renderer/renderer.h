
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer/shapes.h> 

typedef struct RendererContext {
    int32_t windowWidth;
    int32_t windowHeight;

    // OpenGL
    uint32_t VAO;
    uint32_t* VBOS;
    uint32_t shaderProgram;
} RendererContext;

void renderer_init(RendererContext* rendererContext);
void renderer_framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height);


#endif
