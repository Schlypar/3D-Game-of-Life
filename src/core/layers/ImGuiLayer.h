#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Application.h"
#include "Layer.h"

namespace GoL {

class ImGuiLayer : public Layer {
private:
    float time = 0.0f;

public:
    ImGuiLayer()
        : Layer("ImGuiLayer") {
    }

    ~ImGuiLayer() = default;

    void OnAttach() override {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;

        ImGui_ImplGlfw_InitForOpenGL(Application::Get().GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void OnDetach() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void OnUpdate() override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // code goes here
        static bool show_demo = true;

        ImGui::ShowDemoWindow(&show_demo);

        // code end here

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};

}