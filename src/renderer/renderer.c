
#include <renderer/renderer.h>
#include <renderer/shapes.h>
#include <containers/darray.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>

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

void renderer_framebuffer_size_callback(GLFWwindow* window, int32_t width, int32_t height) {
    RendererContext* renderContext = glfwGetWindowUserPointer(window);
    renderContext->windowWidth = width;
    renderContext->windowHeight = height;
    glViewport(0, 0, width, height);
}

void renderer_init_shaders(RendererContext* renderContext) {
    const char* vertexShaderString = readFileToString("src/renderer/shaders/default_vertex.glsl");
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

    const char* fragmentShaderString = readFileToString("src/renderer/shaders/default_fragment.glsl");
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

    renderContext->shaderProgram = glCreateProgram();
    glAttachShader(renderContext->shaderProgram, vertexShader);
    glAttachShader(renderContext->shaderProgram, fragmentShader);
    glLinkProgram(renderContext->shaderProgram);

    glGetProgramiv(renderContext->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(renderContext->shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Failed to link shaders: %s\n", infoLog);
        free((void*)vertexShaderString);
        free((void*)fragmentShaderString);
        return;
    }

    glUseProgram(renderContext->shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

bool renderer_init(RendererContext* rendererContext) {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return true;

    glViewport(0, 0, rendererContext->windowWidth, rendererContext->windowHeight);

    rendererContext->VBOS = darrayCreate(uint32_t*);
    glGenVertexArrays(1, &rendererContext->VAO);
    glBindVertexArray(rendererContext->VAO);
    renderer_setup_basic_shapes(rendererContext);
    renderer_init_shaders(rendererContext);

    return false;
}

