#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <list>
#include <unordered_map>

#include "Layer.h"

namespace GoL {

class ImGuiLayer : public Layer {
public:
    using DisplayFn = std::function<void()>;

private:
    std::unordered_map<std::string, std::list<DisplayFn>> displayFunctions;
    std::string activeSceneName;
    float time = 0.0f;

public:
    ImGuiLayer();

    ~ImGuiLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;

    void SubmitDisplay(std::string sceneName, DisplayFn display);
    void SwitchScene(std::string sceneName);
    void ClearDisplay(std::string sceneName);
};

}