#pragma once

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "InstanceHandler.h"
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

        Shader shader = { "../resources/shaders/test.shader" };
        Shader instanceShader = { "../resources/shaders/instance.shader" };
        shader.Bind();

        auto cube = Cube(glm::vec3 { 0.0f }, glm::vec3 { 0.0f }, 0.5f);
        int cube_ibo = cube.BindIndices();

        auto cursor = Cursor3D();
        cursor.BindIndices();

        InstanceHandler<Cube> cubes;
        for (float i = 5; i < 8; i += 0.5) {
            for (float j = 5; j < 8; j += 0.5) {    
                for (float k = 5; k < 8; k += 0.5) {
                    cubes.AddInstance(glm::vec3(i, j, k), glm::vec3(0.0f), 0.2);
                }
            }
        }

        // auto prism = Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
        // prism.BindIndices();

        IndexBuffer::Init();

        while (!window.ShouldClose()) {
            Parameters::currentFrameTime = static_cast<float>(glfwGetTime());
            Parameters::deltaTime = Parameters::currentFrameTime - Parameters::lastFrameTime;
            Parameters::lastFrameTime = Parameters::currentFrameTime;

            Renderer::Clear();

            Renderer::DrawBasic<Cube>(cube, Parameters::camera, shader);
            // Renderer.Draw<Prism>(prism, Parameters::camera, shader);
            cursor.SetScaleFactor(100);
            Renderer::DrawBasic<Cursor3D>(cursor, Parameters::camera, shader);
            cursor.SetScaleFactor(0.1);
            Renderer::DrawCustom<Cursor3D, Cursor3D::CustomDraw>(cursor, Parameters::camera, shader);

            Renderer::DrawInstanced(cubes, cube_ibo,  Parameters::camera, instanceShader);

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