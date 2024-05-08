#pragma once

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "Window.h"

#include "IndexBuffer.h"

#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "Models/Prism.h"
#include "renderer/Renderer.h"

namespace GoL {

class Application {
public:
    struct Parameters {
        static bool firstMouse;
        static float lastX, lastY;

        static float currentFrameTime;
        static float lastFrameTime;
        static float deltaTime;

        static Camera camera;
    };

private:
    Window window;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1980,
            unsigned int height = 1080
    )
        : window(Window::Settings(title, width, height)) {
    }

    void ConfigureCallbacks(
            void (*error_callback)(int, const char*),
            void (*mouse_callback)(GLFWwindow*, double, double)
    ) {
        // window settings
        window.SetVSync(true);
        window.Configure();
        window.SetErrorCallback(error_callback);
        window.SetCursorPosCallback(mouse_callback);

        // possibly other settings...
    }

    void Run() {
        Renderer renderer;
        Cursor3DRenderer cursor_renderer;

        Shader shader = { "resources/shaders/test.shader" };
        shader.Bind();

        auto cube = Cube(glm::vec3 { 0.0f }, glm::vec3 { 15.0f, 0.0f, 45.0f }, 0.5f);
        cube.BindIndices();

        auto cursor = Cursor3D();
        cursor.BindIndices();

        auto prism = Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
        prism.BindIndices();

        IndexBuffer::Init();

        while (!window.ShouldClose()) {
            Parameters::currentFrameTime = static_cast<float>(glfwGetTime());
            Parameters::deltaTime = Parameters::currentFrameTime - Parameters::lastFrameTime;
            Parameters::lastFrameTime = Parameters::currentFrameTime;

            renderer.Clear();

            renderer.Draw<Cube>(cube, Parameters::camera, shader);
            renderer.Draw<Prism>(prism, Parameters::camera, shader);
            cursor.SetScaleFactor(100);
            renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);
            cursor.SetScaleFactor(0.1);
            cursor_renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);

            window.OnUpdate(Parameters::camera);
            ProcessInput();
        }
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

private:
    void ProcessInput() {
        float cameraSpeed = static_cast<float>(2.5 * Parameters::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::LEFT, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::UP, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::DOWN, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(0.0f, 10.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(0.0f, -10.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            Parameters::firstMouse = false;
        }
        // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 == GLFW_PRESS)) {
        //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //     glfwSetCursorPosCallback(window, mouse_callback);
        // }
    }
};

}