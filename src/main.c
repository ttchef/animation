#include <zig/tokenizer.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <renderer/renderer.h> 

int main(void) {
    RendererContext renderContext = { 
        .windowWidth = 800,
        .windowHeight = 600,
    };

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(renderContext.windowWidth, renderContext.windowHeight, "Animation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return -1;
    }

    renderer_init(&renderContext);
    glfwSetWindowUserPointer(window, &renderContext);
    glfwSetFramebufferSizeCallback(window, renderer_framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        renderer_draw_triangle(&renderContext, (Color){0.7f, 0.2f, 0.1f, 1.0f});

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
