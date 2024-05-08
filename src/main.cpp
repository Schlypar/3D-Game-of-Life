#include <stdio.h>

#include "Application.h"

static void error_callback(int error, const char* description);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main() {
    GoL::Application application;
    application.ConfigureCallbacks(error_callback, mouse_callback);
    application.Run();
}

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (GoL::Application::Parameters::firstMouse) {
        GoL::Application::Parameters::lastX = xpos;
        GoL::Application::Parameters::lastY = ypos;
        GoL::Application::Parameters::firstMouse = false;
    }

    glm::vec2 offset(xpos - GoL::Application::Parameters::lastX, GoL::Application::Parameters::lastY - ypos);
    GoL::Application::Parameters::lastX = xpos;
    GoL::Application::Parameters::lastY = ypos;

    GoL::Application::Parameters::camera.ProcessMouseMovement(offset.x, offset.y);
}
