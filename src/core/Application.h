#pragma once

#include "precompiled.h"

#include "LayerStack.h"

#include "Window.h"

#include "events/Event.h"
#include "events/EventQueue.h"

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"

#include "layers/ImGuiLayer.h"

namespace GoL {

class Application {
private:
    bool isRunning = true;
    Window window;
    LayerStack layerStack;
    ImGuiLayer* imGuiLayer;
    EventQueue eventQueue;

    static Application* instance;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1920,
            unsigned int height = 1080
    );

    ~Application();

    Window& GetWindow();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void SubmitToImgui(ImGuiLayer::DisplayFn display);

    static Application& Get() {
        return *instance;
    }

    void Run();

    void OnEvent(Event* e);

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnKeyPress(KeyPressedEvent& e);
};
}
