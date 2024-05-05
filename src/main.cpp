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
#include "Models/Prism.h"
#include "glad/gl.h"
#include "renderer/Renderer.h"
#include "IndexBuffer.h"

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

bool firstMouse = true;
float lastX, lastY;
GoL::Camera* cam = nullptr;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cam == nullptr) {
        return;
    }
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    glm::vec2 offset(xpos - lastX, lastY - ypos); 
    lastX = xpos;
    lastY = ypos;

    cam->ProcessMouseMovement(offset.x, offset.y);
}

void processInput(GLFWwindow* window, GoL::Camera& camera) {

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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.ProcessKeyboard(GoL::CameraMovement::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessMouseMovement(-10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessMouseMovement(10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.ProcessMouseMovement(-10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.ProcessMouseMovement(10.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.ProcessMouseMovement(0.0f, 10.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.ProcessMouseMovement(0.0f, -10.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, NULL);
        firstMouse = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 == GLFW_PRESS)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    GoL::Shader shader = { "resources/shaders/test.shader" };
    shader.Bind();

    GoL::Camera camera = GoL::Camera({ 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, width, height);
    cam = &camera;
    GoL::Renderer renderer;
    GoL::Cursor3DRenderer cursor_renderer;

    auto cube = GoL::Cube(glm::vec3 { 0.0f }, glm::vec3 { 15.0f, 0.0f, 45.0f }, 0.5f);
    cube.BindIndices();

    auto cursor = GoL::Cursor3D();
    cursor.BindIndices();
    
    auto prism = GoL::Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
    prism.BindIndices();

    GoL::IndexBuffer::Init();

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera);

        renderer.Clear();

        renderer.Draw<GoL::Cube>(cube, camera, shader);
        renderer.Draw<GoL::Prism>(prism, camera, shader);
        cursor.SetScaleFactor(100);
        renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);
        cursor.SetScaleFactor(0.1);
        cursor_renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
