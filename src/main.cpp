#include <cstddef>
#include <stdio.h>
#include <stdlib.h>

#include "renderer/Renderer.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(640, 480, "Main", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    float positions[] = {
        // 1 point
        -0.5f,
        -0.5f,
        // 2 point
        0.5f,
        -0.5f,
        // 3 point
        0.5f,
        0.5f,
        // 4 point
        -0.5f,
        0.5f,
    };

    unsigned int indices[] = {
        // 1 triangle
        0,
        1,
        2,
        // 2 triangle
        2,
        3,
        0
    };

    GoL::VertexBuffer vertexBuffer = { positions, 2 * 4 * sizeof(float) };

    Gol::VertexBufferLayout layout;
    layout.Push<float>(2);

    GoL::VertexArray vertexArray;
    vertexArray.AddBuffer(vertexBuffer, layout);

    GoL::IndexBuffer indexBuffer = { indices, 6 };

    GoL::Shader shader = { "resources/shaders/test_vertex.shader", "resources/shaders/test_fragment.shader" };

    GoL::Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        renderer.Clear();

        renderer.Draw(vertexBuffer, indexBuffer, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
