
#include <renderer/renderer.h>
#include <renderer/shapes.h>
#include <containers/darray.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>

#define FIREF_IMPL
#include <firef/firef.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> 

char* readFileToString(const char *path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed reading file: %s\n", path);
        return NULL;;
    }

    fseek(file, 0, SEEK_END);
    int32_t size = ftell(file);
    rewind(file);

    char* buf = malloc(size + 1);
    if (!buf) {
        fprintf(stderr, "Failed allocating for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    fread(buf, 1, size, file);
    buf[size] = '\0';

    fclose(file);
    
    return buf;
}

void renderer_resize(Renderer* renderer, size_t width, size_t height) {
    renderer->windowWidth = width;
    renderer->windowHeight = height;
    glViewport(0, 0, width, height);
}

void renderer_init_shaders(Renderer* renderer, usize index, const char* vPath, const char* fPath) {
    const char* vertexShaderString = readFileToString(vPath);
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
    glCompileShader(vertexShader);
    
    int32_t success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader Compilation Failed: %s!\n", infoLog);
        free((void*)vertexShaderString);
        return;
    }

    const char* fragmentShaderString = readFileToString(fPath);
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment Shader Compilation Failed: %s!\n", infoLog);
        free((void*)vertexShaderString);
        free((void*)fragmentShaderString);
        return;
    }

    renderer->programs[index] = glCreateProgram();
    glAttachShader(renderer->programs[index], vertexShader);
    glAttachShader(renderer->programs[index], fragmentShader);
    glLinkProgram(renderer->programs[index]);

    glGetProgramiv(renderer->programs[index], GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(renderer->programs[index], 512, NULL, infoLog);
        fprintf(stderr, "Failed to link shaders: %s\n", infoLog);
        free((void*)vertexShaderString);
        free((void*)fragmentShaderString);
        return;
    }

    glUseProgram(renderer->programs[index]);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

bool renderer_init(Renderer* renderer, GLADloadproc loader) {
    if (!gladLoadGLLoader(loader)) return true;

    glViewport(0, 0, renderer->windowWidth, renderer->windowHeight);

    renderer->VAOS = darrayCreate(gl_u32);
    renderer->VBOS = darrayCreate(gl_u32);
    renderer->EBOS = darrayCreate(gl_u32);
    renderer->programs = darrayCreate(gl_u32);
    renderer->textures = darrayCreate(gl_u32);
    renderer->meshes = darrayCreate(gl_u32);

    renderer_setup_basic_shapes(renderer);
    renderer_init_shaders(renderer, DEFAULT_PROGRAM, "src/renderer/shaders/default_vertex.glsl", "src/renderer/shaders/default_fragment.glsl");
    darrayPush(renderer->programs, (gl_u32)0);
    renderer_init_shaders(renderer, TEXTURE_PROGRAM, "src/renderer/shaders/texture_vertex.glsl", "src/renderer/shaders/texture_fragment.glsl");

    return false;
}

TextureID renderer_load_texture(Renderer* renderer, const char *path) {
    gl_u32 texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    i32 width, height, channels;
    stbi_set_flip_vertically_on_load(1);    u8* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        gl_u32 format = GL_RGB;
        gl_u32 internalFormat = GL_RGB;

        if (channels == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA;
        } else if (channels == 1) {
            format = GL_RED;
            internalFormat = GL_RED;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return -1;
    }

    stbi_image_free(data);
    darrayPush(renderer->textures, texture);

    return (TextureID)darrayLength(renderer->textures) - 1;
}

Mesh renderer_load_obj(struct Renderer *renderer, const char *path) {
    Obj mesh = load_obj(path);
    darrayPush(renderer->VAOS, (gl_u32){0});
    darrayPush(renderer->VBOS, (gl_u32){0});
    darrayPush(renderer->EBOS, (gl_u32){0});

    u32 index = darrayLength(renderer->VAOS) - 1;
    u32 index_count = mesh.index_count;
    glGenVertexArrays(1, &renderer->VAOS[index]);
    glBindVertexArray(renderer->VAOS[index]);

    glGenBuffers(1, &renderer->VBOS[index]);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBOS[index]);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertex_count * sizeof(f32), mesh.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &renderer->EBOS[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBOS[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index_count * sizeof(u32), mesh.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(5 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    free_obj(&mesh);
    return (Mesh){index, index_count};
}

