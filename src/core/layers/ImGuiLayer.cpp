#include "ImGuiLayer.h"

#include "Application.h"

namespace GoL {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {
}

void ImGuiLayer::OnAttach() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui_ImplGlfw_InitForOpenGL(Application::Get().GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
    OPENGL_INFO("Imgui initialized with opengl version 410");
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::OnUpdate() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto& display : displayFunctions[activeSceneName]) {
        display();
    }

    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::SubmitDisplay(std::string sceneName, DisplayFn display) {
    displayFunctions[sceneName].push_back(display);
}

void ImGuiLayer::SwitchScene(std::string sceneName) {
    this->activeSceneName = sceneName;
}

void ImGuiLayer::ClearDisplay(std::string sceneName) {
    if (activeSceneName == sceneName) {
        activeSceneName = "";
    }

    displayFunctions.erase(sceneName);
}

}
