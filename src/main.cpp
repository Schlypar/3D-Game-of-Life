#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"
#include "layers/Sandbox.h"

#include "renderer/CLHandler.h"

#include <fstream>

const std::string MainSceneName = "main";
const std::string SandboxSceneName = "sandbox";

int main() {
    GoL::Application app;

#ifdef DEBUG
    GoL::OPENGL_CHANGE_LEVEL(spdlog::level::trace);
    GoL::CORE_CHANGE_LEVEL(spdlog::level::trace);
#endif

    std::ofstream file("/home/den/Projectes/3D-Game-of-Life/res");

    auto pls = GoL::CLHandler::GetPlatforms();
    auto devs2 = GoL::CLHandler::GetDevicesForPlatform(pls[0]);
    auto devs3 = GoL::CLHandler::GetDevicesForPlatform(pls[1]);

    auto h = GoL::CLHandler(pls[0], devs2[0]);

    app.NewScene(MainSceneName);
    app.NewScene(SandboxSceneName);

    app.PushLayer(MainSceneName, new GoL::MainLayer());
    app.PushLayer(SandboxSceneName, new GoL::Sandbox(SandboxSceneName, 10));

    bool showDemoWindow = true;
    app.SubmitToImgui([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } }, MainSceneName);

    app.SwitchScene(MainSceneName);
    app.Run();

    return 0;
}
