#include <stdio.h>

#include "Application.h"
#include "layers/MainLayer.h"

int main() {
    GoL::Application application;
    GoL::Layer* mainLayer = new GoL::MainLayer();
    application.PushLayer(mainLayer);
    application.Run();

    return 0;
}