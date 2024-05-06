#include "Application.h"

namespace GoL {

// CAMERA DEFAULTS [CHANGE THIS TO CHANGE INITIAL CAMERA]
constexpr glm::vec3 CAMERA_POSITION = { 0.0f, 0.0f, 3.0f };
constexpr glm::vec3 WOLRD_UP = { 0.0f, 1.0f, 0.0f };
constexpr float WINDOW_WIDTH = 1980;
constexpr float WINDOW_HEIGHT = 1080;

bool Application::Parameters::firstMouse = true;
float Application::Parameters::lastX = 0;
float Application::Parameters::lastY = 0;

float Application::Parameters::currentFrameTime = 0;
float Application::Parameters::lastFrameTime = 0;
float Application::Parameters::deltaTime = 0;

Camera Application::Parameters::camera = Camera(
        CAMERA_POSITION,
        WOLRD_UP,
        WINDOW_WIDTH,
        WINDOW_HEIGHT
);

}