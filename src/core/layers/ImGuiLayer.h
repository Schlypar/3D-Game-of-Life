#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>

#include "Layer.h"

namespace GoL {

class ImGuiLayer : public Layer {
public:
    using DisplayFn = std::function<void()>;

private:
    std::vector<DisplayFn> displayFunctions;
    float time = 0.0f;

public:
    ImGuiLayer();

    ~ImGuiLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;

    void SubmitDisplay(DisplayFn display);
};

}