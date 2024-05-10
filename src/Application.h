#pragma once

#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "Models/Prism.h"
#include "Renderer.h"
#include "Window.h"
#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/MouseEvent.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

namespace GoL {

static bool captureMouse = true;

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
    bool isRunning = true;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1980,
            unsigned int height = 1080
    )
        : window(Window::Data(title, width, height)) {
        this->window.SetEventCallback(
                [this](Event& e) {
                    this->OnEvent(e);
                }
        );
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
            Parameters::currentFrameTime = static_cast<float>(glfwGetTime());
            Parameters::deltaTime = Parameters::currentFrameTime - Parameters::lastFrameTime;
            Parameters::lastFrameTime = Parameters::currentFrameTime;

            renderer.Clear();

            renderer.Draw<Cube>(cube, Parameters::camera, shader);
            // renderer.Draw<Prism>(prism, Parameters::camera, shader);
            cursor.SetScaleFactor(100);
            renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);
            cursor.SetScaleFactor(0.1);
            cursor_renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);

            window.OnUpdate();
        }

        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    void OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));
    }

    bool OnWindowClose(WindowCloseEvent& e) {
        isRunning = false;
        return true;
    }
};

}