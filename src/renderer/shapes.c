
#include <renderer/shapes.h>
#include <renderer/renderer.h>

float triangle_vertices[9] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

void renderer_setup_basic_shapes(struct RendererContext *renderContext) {
    glGenBuffers(1, &renderContext->VBOS[0]);
    glBindBuffer(GL_ARRAY_BUFFER, renderContext->VBOS[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void renderer_draw_triangle(RendererContext *renderContext, Color color) {
    glUseProgram(renderContext->shaderProgram);
    int32_t colorLoc = glGetUniformLocation(renderContext->shaderProgram, "uColor");
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    glBindVertexArray(renderContext->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

