#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"
#include "layers/Sandbox.h"

int main() {
    GoL::Application application;

    GoL::OPENGL_CHANGE_LEVEL(spdlog::level::warn);
    GoL::CORE_CHANGE_LEVEL(spdlog::level::trace);

    // application.PushLayer(new GoL::MainLayer());
    application.PushLayer(new GoL::Sandbox());

    // bool showDemoWindow = true;
    // application.SubmitToImgui([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } });

    application.Run();

    return 0;
}
