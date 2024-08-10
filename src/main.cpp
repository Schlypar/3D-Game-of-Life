#include "Application.h"

#include "layers/ImGuiLayer.h"
#include "layers/MainLayer.h"
#include "layers/Sandbox.h"

#include "renderer/CL/Device.h"
#include "renderer/CL/Platform.h"
#include <CL/cl.h>

#include <fstream>

const std::string MainSceneName = "main";
const std::string SandboxSceneName = "sandbox";

int selectPlatform(std::vector<GoL::CL::Platform> plats) {
    int i = 0;
    for (auto& p : plats) {
        for (auto& ext : p.extensions) {
            if (ext == "cl_khr_gl_sharing") {
                return i;
            }
        }

        i++;
    }

    return -1;
}

int main() {
    GoL::Application app;

#ifdef DEBUG
    GoL::OPENGL_CHANGE_LEVEL(spdlog::level::trace);
    GoL::CORE_CHANGE_LEVEL(spdlog::level::trace);
#endif

    std::ofstream file("/home/den/Projectes/3D-Game-of-Life/res");

    auto platformsCL = GoL::CL::Platform::Enumerate();
    if (platformsCL.size() < 1) {
        throw std::runtime_error("no CL platforms in the system!");
    }

    bool has_cl_khr_gl_sharing = true;
    auto platformWithExtension = selectPlatform(platformsCL);
    if (platformWithExtension < 0) {
        // in code there is actually support for those without cl_khr_gl_sharing but i`m tired
        // throw std::runtime_error("no openCL platform with cl_khr_gl_sharing extension found");
        platformWithExtension = 0;
        has_cl_khr_gl_sharing = false;
    }

    auto devicesCL = GoL::CL::Device::Enumerate(platformsCL[platformWithExtension].id, CL_DEVICE_TYPE_GPU);
    if (devicesCL.size() < 1) {
        throw std::runtime_error("no CL devices for selected platform found in the system!");
    }

    app.NewScene(MainSceneName);
    app.NewScene(SandboxSceneName);

    app.PushLayer(MainSceneName, new GoL::MainLayer({ platformsCL[platformWithExtension], devicesCL[0], has_cl_khr_gl_sharing }));
    app.PushLayer(SandboxSceneName, new GoL::Sandbox(SandboxSceneName, 10));

    bool showDemoWindow = true;
    app.SubmitToImgui([&showDemoWindow]() { if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); } }, MainSceneName);

    app.SwitchScene(MainSceneName);
    app.Run();

    return 0;
}
