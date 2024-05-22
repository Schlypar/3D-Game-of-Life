
#include "Application.h"
#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"

int main() {
    GoL::Application application;
    application.PushLayer(new GoL::MainLayer());

    GoL::ImGuiLayer* imGuiLayer = new GoL::ImGuiLayer();
    bool showDemoWindow = true;
    imGuiLayer->SubmitDisplay([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } });
    application.PushOverlay(imGuiLayer);

    application.Run();

    return 0;
}