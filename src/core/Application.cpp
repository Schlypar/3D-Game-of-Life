#include <cassert>
#include <cstdlib>

#include "Logger.h"

#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"

#define GET_SCENE_OR_RETURN_FALSE(scenes, sceneName) \
    LayerStack* scene; \
    if (!scenes.contains(sceneName)) { \
        return false; \
    } \
    scene = scenes[sceneName];

namespace GoL {

Application* Application::instance = nullptr;

Application::Application(
        std::string title,
        unsigned int width,
        unsigned int height
)
    : window(Window::Data(title, width, height))
    , scenes()
    , activeScene(nullptr)
    , imGuiLayer(new ImGuiLayer()) 
    , eventQueue([this](Event* e) {
        auto scene = this->activeScene.load();
        if (scene == nullptr) {
            return;
        }

        EventDispatcher dispatcher(*e);

        dispatcher.Dispatch<WindowCloseEvent>(BIND_MEMBER_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Application::OnKeyPress));

        for (auto it = scene->rbegin(); it != scene->rend(); it++) {
            if (e->Handled) {
                break;
            }

            (*it)->OnEvent(e);
        }
    }) {
    // only 1 appplication instance may be
    assert(!Application::instance);
    Application::instance = this;

    Logger::Init();

    this->window.SetEventCallback(
            [this](Event* e) {
                this->OnEvent(e);
            }
    );
    this->window.Configure();
    this->imGuiLayer->OnAttach();
    this->eventQueue.Run();
}

Application::~Application() {
    this->eventQueue.Stop();
    Application::instance = nullptr;
}

Window& Application::GetWindow() {
    return this->window;
}

bool Application::PushLayer(std::string sceneName, Layer* layer) {
    GET_SCENE_OR_RETURN_FALSE(this->scenes, sceneName);
    if (scene == activeScene.load()) {
        eventQueue.Stop();
        
    }

    layer->Scene() = sceneName;
    scene->PushLayer(layer);
    layer->OnAttach();

    if (scene == activeScene.load()) {
        eventQueue.Run();
        
    }

    return true;
}

bool Application::PushOverlay(std::string sceneName, Layer* overlay) {
    GET_SCENE_OR_RETURN_FALSE(this->scenes, sceneName);
    if (scene == activeScene.load()) {
        eventQueue.Stop();
        
    }

    overlay->Scene() = sceneName;
    scene->PushOverlay(overlay);
    overlay->OnAttach();

    if (scene == activeScene.load()) {
        eventQueue.Run();    
    }
    
    return true;
}

void Application::SubmitToImgui(ImGuiLayer::DisplayFn display, std::string sceneName) {
    auto activeScenePtr = activeScene.load();
    if (sceneName == "") {
        for (auto& scene : this->scenes) {
            if (scene.second == activeScenePtr) {
                sceneName = scene.first;
            }
        }
    }
    this->imGuiLayer->SubmitDisplay(sceneName, display);
}

void Application::Run() {
    while (isRunning) {

        auto scene = this->activeScene.load();
        for (auto it = scene->begin(); it != scene->end(); it++) {
            (*it)->OnUpdate();
        }

        window.OnUpdate();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void Application::OnEvent(Event* e) {
    eventQueue.Push(e);
}

bool Application::NewScene(std::string sceneName) {
    if (this->scenes.contains(sceneName)) {
        return false;
    }

    this->scenes[sceneName] = new LayerStack();
    this->scenes[sceneName]->PushOverlay(imGuiLayer);
    return true;
}

std::list<std::string> Application::ListScenes() {
    auto sceneNames = std::list<std::string>(this->scenes.size());
    for (auto &scene : this->scenes) {
        sceneNames.push_back(scene.first);
    }

    return sceneNames;
}

bool Application::DeleteScene(std::string sceneName) {
    GET_SCENE_OR_RETURN_FALSE(this->scenes, sceneName);
    
    if (scene == activeScene) {
        return false;
    }

    this->scenes.erase(sceneName);
    return true;
}

bool Application::SwitchScene(std::string sceneName) {
    GET_SCENE_OR_RETURN_FALSE(this->scenes, sceneName);

    if (scene == this->activeScene) {
        return true;
    }

    eventQueue.Stop();
    this->activeScene.store(scene);
    eventQueue.Run();

    this->imGuiLayer->SwitchScene(sceneName);

    return true;
}

// private:
bool Application::OnWindowClose(WindowCloseEvent& e) {
    isRunning = false;
    return true;
}

bool Application::OnKeyPress(KeyPressedEvent& e) {
    static bool mainLayerMouseMoveHandle = true;

    if (e.GetKeyCode() == GLFW_KEY_ESCAPE) {
        for (auto layer : *activeScene) {
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
