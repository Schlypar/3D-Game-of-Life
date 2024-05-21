#pragma once

#include "LayerStack.h"
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
    bool isRunning = true;
    Window window;
    LayerStack layerStack;

    // Camera camera;

    // bool firstMouse = true;
    // float lastX = 0, lastY = 0;

    // float currentFrameTime = 0;
    // float lastFrameTime = 0;
    // float deltaTime = 0;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1920,
            unsigned int height = 1080
    )
        : window(Window::Data(title, width, height))
    // , camera({ 0.0f, 0.0f, -3.0f }, { 0.0f, 1.0f, 0.0f }, 1920.0f, 1080.0f) {
    {
        this->window.SetEventCallback(
                [this](Event& e) {
                    this->OnEvent(e);
                }
        );
        this->window.Configure();
    }

    void PushLayer(Layer* layer) {
        this->layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Run() {
        while (isRunning) {

            for (auto it = layerStack.begin(); it != layerStack.end(); it++) {
                (*it)->OnUpdate();
            }

            window.OnUpdate();
        }

        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    void OnEvent(Event& e) {
        std::cout << e << std::endl;
        EventDispatcher dispatcher(e);

        // probably make it async in the future ?
        dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));

        for (auto it = layerStack.rbegin(); it != layerStack.rend(); it++) {
            if (e.Handled) {
                break;
            }

            (*it)->OnEvent(e);
        }

        // TODO
        // dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Application::OnKeyPress));
        // dispatcher.Dispatch<MouseMovedEvent>(BIND_MEMBER_EVENT_FN(Application::OnMouseMove));
    }

private:
    bool OnWindowClose(WindowCloseEvent& e) {
        isRunning = false;
        return true;
    }
};

}