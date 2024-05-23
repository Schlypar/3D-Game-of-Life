
#include "Application.h"
#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"

int main() {
    GoL::Application application;
    application.PushLayer(new GoL::MainLayer());

    bool showDemoWindow = true;
    application.SubmitToImgui([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } });

    application.Run();

    return 0;
}