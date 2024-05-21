#pragma once

#include "LayerStack.h"

#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/Event.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace GoL {

class Application {
public:
private:
    bool isRunning = true;
    Window window;
    LayerStack layerStack;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1920,
            unsigned int height = 1080
    )
        : window(Window::Data(title, width, height)) {
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

    void PushOverlay(Layer* overlay) {
        this->layerStack.PushOverlay(overlay);
        overlay->OnAttach();
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
        EventDispatcher dispatcher(e);

        // probably make it async in the future ?
        dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));

        for (auto it = layerStack.rbegin(); it != layerStack.rend(); it++) {
            if (e.Handled) {
                break;
            }

            (*it)->OnEvent(e);
        }

        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Application::OnKeyPress));

        // TODO
        // dispatcher.Dispatch<MouseMovedEvent>(BIND_MEMBER_EVENT_FN(Application::OnMouseMove));
    }

private:
    bool OnWindowClose(WindowCloseEvent& e) {
        isRunning = false;
        return true;
    }

    bool OnKeyPress(KeyPressedEvent& e) {
        if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            return true;
        }

        return false;
    }
};

}