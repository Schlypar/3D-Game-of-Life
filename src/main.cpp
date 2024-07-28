#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"
#include "layers/Sandbox.h"

const std::string MainSceneName = "main";
const std::string SandboxSceneName = "sandbox";

int main() {
    GoL::Application app;

    GoL::OPENGL_CHANGE_LEVEL(spdlog::level::warn);
    GoL::CORE_CHANGE_LEVEL(spdlog::level::trace);

    app.NewScene(MainSceneName);
    app.NewScene(SandboxSceneName);

    app.PushLayer(MainSceneName, new GoL::MainLayer());
    app.PushLayer(SandboxSceneName, new GoL::Sandbox());

    bool showDemoWindow = true;
    app.SubmitToImgui([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } }, MainSceneName);

    app.SwitchScene(MainSceneName);
    app.Run();

    return 0;
}
