#pragma once

#include <atomic>
#include <unordered_map>
#include <list>
#include <string>

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
    std::unordered_map<std::string, LayerStack*> scenes;
    std::atomic<LayerStack*> activeScene;
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

    /**
    * @name GetWindow - Gives reference to Window class
    * @return Window - field inside Application
    */
    Window& GetWindow();

    /**
    * @name PushLayer - Adds layer to the LayerStack of Application
    * @param layer -  Layer to be added
    * @return bool
    */
    bool PushLayer(std::string sceneName, Layer* layer);

    /**
    * @name PushOverlay - Adds overlay to the LayerStack of Application. Overlays are drawn on top of layers.
    * @param overlay -  Overlay to be added
    * @return bool
    */
    bool PushOverlay(std::string sceneName, Layer* overlay);

    /**
    * @name SubmitToImgui - Tells ImGui to display in Debug window
    * @param display -  ImGui widget ti be displayed
    * @return void
    */
    void SubmitToImgui(ImGuiLayer::DisplayFn display, std::string sceneName = "");

    /**
    * @name Get - Gives instance of Application singleton class
    * @return Application - instance of Application
    */
    static Application& Get() {
        return *instance;
    }

    /**
    * @name Run - Runs the Application till it closes
    * @return void
    */
    void Run();

    /**
    * @name OnEvent - Handles events for Application
    * @param e - Event to be handled
    * @return void
    */
    void OnEvent(Event* e);

    /**
    * @name NewScene - Adds new scene
    * @param sceneName - scene name
    * @return bool - true if scene was creates, false if scene with such name already exists
    */
    bool NewScene(std::string sceneName);

    /**
    * @name ListScenes - List existing scenes
    * @return std::list<std::string>
    */
    std::list<std::string> ListScenes();

    /**
    * @name DeleteScene - Deletes scene from Application
    * @param sceneName - scene name
    * @return bool - true if scene was deletes, false if no such scene exists 
    */
    bool DeleteScene(std::string sceneName);

    /**
    * @nameSwitchScene - Sets specified scene active: displays its layers and use theirs callbacks
    * @param sceneName - scene name
    * @return bool - true if scene was switched on, false if no such scene exists 
    */
    bool SwitchScene(std::string sceneName);

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnKeyPress(KeyPressedEvent& e);
};
}
