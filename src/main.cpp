#include <stdio.h>

#include "Application.h"
#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"

int main() {
    GoL::Application application;
    application.PushLayer(new GoL::MainLayer());
    application.PushOverlay(new GoL::ImGuiLayer());
    application.Run();

    return 0;
}