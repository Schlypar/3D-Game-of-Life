#include <cstddef>
#include <glm/ext/matrix_projection.hpp>
#include <glm/fwd.hpp>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "Camera.h"
#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "glad/gl.h"
#include "renderer/Renderer.h"

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow* window, GoL::Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessMouseMovement(-10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessMouseMovement(10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessMouseMovement(0.0f, -10.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camera.ProcessMouseMovement(0.0f, 10.0f);
    }
}

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    int width = 1920;
    int height = 1080;

    window = glfwCreateWindow(width, height, "Main", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GoL::Shader shader = { "resources/shaders/test.shader" };
    shader.Bind();

    GoL::Camera camera = GoL::Camera({ 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, width, height, GoL::YAW, GoL::PITCH);
    GoL::Renderer renderer;

    std::shared_ptr<GoL::Model> cube = std::make_shared<GoL::Cube>(
            glm::vec3 { 0.0f }, glm::vec3 { 15.0f, 0.0f, 45.0f }, 0.5f
    );
    std::shared_ptr<GoL::Model> cursor = std::make_shared<GoL::Cursor3D>();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera);

        renderer.Clear();

        renderer.Draw(cube, camera, shader);
        renderer.Draw(cursor, camera, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
