
#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h> 
#include <stdlib.h>
#include <stdbool.h>

#include <glad/glad.h>

#include <renderer/shapes.h> 

enum {
    DEFAULT_PROGRAM,
    TEXTURE_PROGRAM,
};

typedef struct Renderer {
    usize windowWidth;
    usize windowHeight;

    // OpenGL
    gl_u32* VAOS;
    gl_u32* VBOS;
    gl_u32* EBOS;
    gl_u32* programs;
    gl_u32* textures;
    gl_u32* meshes;
} Renderer;

bool renderer_init(Renderer* renderer, GLADloadproc loader);
void renderer_resize(Renderer* renderer, size_t width, size_t height);
TextureID renderer_load_texture(Renderer* renderer, const char* path);
Mesh renderer_load_obj(Renderer* renderer, const char* path);

#endif
