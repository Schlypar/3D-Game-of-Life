#pragma once

#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "Models/Prism.h"
#include "Renderer.h"

#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

namespace GoL {

class Application {
public:
private:
    Window window;
    Camera camera;

    bool isRunning = true;

    bool firstMouse = true;
    float lastX = 0, lastY = 0;

    float currentFrameTime = 0;
    float lastFrameTime = 0;
    float deltaTime = 0;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1980,
            unsigned int height = 1080
    )
        : window(Window::Data(title, width, height))
        , camera({ 0.0f, 0.0f, -3.0f }, { 0.0f, 1.0f, 0.0f }, 1920.0f, 1080.0f) {
        this->window.SetEventCallback(
                [this](Event& e) {
                    this->OnEvent(e);
                }
        );
        this->window.Configure();
    }

    void Run() {
        Renderer renderer;
        Cursor3DRenderer cursor_renderer;

        Shader shader = { "resources/shaders/test.shader" };
        shader.Bind();

        auto cube = Cube(glm::vec3 { 0.0f }, glm::vec3 { 0.0f }, 0.5f);
        cube.BindIndices();

        auto cursor = Cursor3D();
        cursor.BindIndices();

        auto prism = Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
        prism.BindIndices();

        IndexBuffer::Init();

        while (isRunning) {
            currentFrameTime = static_cast<float>(glfwGetTime());
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            renderer.Clear();

            renderer.Draw<Cube>(cube, camera, shader);
            // renderer.Draw<Prism>(prism, camera, shader);
            cursor.SetScaleFactor(100);
            renderer.Draw<Cursor3D>(cursor, camera, shader);
            cursor.SetScaleFactor(0.1);
            cursor_renderer.Draw<Cursor3D>(cursor, camera, shader);

            window.OnUpdate();
        }

        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    void OnEvent(Event& e) {
        std::cout << e << std::endl;
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Application::OnKeyPressed));
    }

    bool OnWindowClose(WindowCloseEvent& e) {
        isRunning = false;
        return true;
    }

    bool OnKeyPressed(KeyPressedEvent& e) {
        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        if (e.GetKeyCode() == GLFW_KEY_W) {
            camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_S) {
            camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_A) {
            camera.ProcessKeyboard(GoL::CameraMovement::LEFT, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_D) {
            camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) {
            camera.ProcessKeyboard(GoL::CameraMovement::UP, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT_CONTROL) {
            camera.ProcessKeyboard(GoL::CameraMovement::DOWN, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_Q) {
            camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_E) {
            camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT) {
            camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_RIGHT) {
            camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_UP) {
            camera.ProcessMouseMovement(0.0f, 10.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_DOWN) {
            camera.ProcessMouseMovement(0.0f, -10.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            firstMouse = false;
        }

        return true;
    }
};

}