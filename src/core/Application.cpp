#include <cassert>
#include <cstdlib>

#include "Logger.h"

#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"

namespace GoL {

Application* Application::instance = nullptr;

Application::Application(
        std::string title,
        unsigned int width,
        unsigned int height
)
    : window(Window::Data(title, width, height))
    , layerStack()
    , imGuiLayer(new ImGuiLayer()) {
    // only 1 appplication instance may be
    assert(!Application::instance);
    Application::instance = this;

    Logger::Init();

    this->window.SetEventCallback(
            [this](Event& e) {
                this->OnEvent(e);
            }
    );
    this->window.Configure();
    this->layerStack.PushOverlay(this->imGuiLayer);
    this->imGuiLayer->OnAttach();
}

Application::~Application() {
    Application::instance = nullptr;
}

Window& Application::GetWindow() {
    return this->window;
}

void Application::PushLayer(Layer* layer) {
    this->layerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* overlay) {
    this->layerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Application::SubmitToImgui(ImGuiLayer::DisplayFn display) {
    this->imGuiLayer->SubmitDisplay(display);
}

void Application::Run() {
    while (isRunning) {

        for (auto it = layerStack.begin(); it != layerStack.end(); it++) {
            (*it)->OnUpdate();
        }

        window.OnUpdate();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);

    // probably make it async in the future ?
    dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));

    dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Application::OnKeyPress));

    for (auto it = layerStack.rbegin(); it != layerStack.rend(); it++) {
        if (e.Handled) {
            break;
        }

        (*it)->OnEvent(e);
    }
}

// private:
bool Application::OnWindowClose(WindowCloseEvent& e) {
    isRunning = false;
    return true;
}

bool Application::OnKeyPress(KeyPressedEvent& e) {
    static bool mainLayerMouseMoveHandle = true;

    if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
        for (auto layer : layerStack) {
            auto mainlayer = dynamic_cast<MainLayer*>(layer);
            if (mainlayer) {
                mainLayerMouseMoveHandle = !mainLayerMouseMoveHandle;
                mainlayer->SetMouseMoveHandler(mainLayerMouseMoveHandle);
            }
        }
    }

    return false;
}

}
