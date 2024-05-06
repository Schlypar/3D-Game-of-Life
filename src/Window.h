#pragma once

#include "Camera.h"
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <string>

namespace GoL {

// static void error_callback(int error, const char* description) {
//     fprintf(stderr, "Error: %s\n", description);
// }

// static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//     if (cam == nullptr) {
//         return;
//     }
//     if (firstMouse) {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     glm::vec2 offset(xpos - lastX, lastY - ypos);
//     lastX = xpos;
//     lastY = ypos;

//     cam->ProcessMouseMovement(offset.x, offset.y);
// }

// static void processInput(GLFWwindow* window, GoL::Camera& camera) {
//     float cameraSpeed = static_cast<float>(2.5 * deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::LEFT, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::UP, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
//         camera.ProcessKeyboard(GoL::CameraMovement::DOWN, deltaTime);
//     }
//     if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(-10.0f, 0.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(10.0f, 0.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(-10.0f, 0.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(10.0f, 0.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(0.0f, 10.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//         camera.ProcessMouseMovement(0.0f, -10.0f);
//     }
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//         glfwSetCursorPosCallback(window, NULL);
//         firstMouse = false;
//     }
//     if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 == GLFW_PRESS)) {
//         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//         glfwSetCursorPosCallback(window, mouse_callback);
//     }
// }

class Window {
public:
    struct Settings {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        Settings(
                std::string title = "Game of Life",
                unsigned int width = 1920,
                unsigned int height = 1080
        )
            : Title(title)
            , Width(width)
            , Height(height) {
        }
    };

private:
    GLFWwindow* window;
    Settings settings;
    bool isVsync;

public:
    Window(const Settings& settings) {
        if (!glfwInit()) {
            exit(EXIT_FAILURE);
        }
        this->window = glfwCreateWindow(
                settings.Width,
                settings.Height,
                settings.Title.c_str(),
                NULL,
                NULL
        );
        if (!this->window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(this->window);
        gladLoadGL(glfwGetProcAddress);
        this->settings = settings;
    }

    ~Window() {
        Close();
    }

    void Configure() {
        glEnable(GL_DEPTH_TEST);

        // glEnable(GL_CULL_FACE);
        // glFrontFace(GL_CCW);
        // glCullFace(GL_BACK);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void OnUpdate(GoL::Camera& camera) {
        glfwPollEvents();
        glfwSwapBuffers(this->window);
    }

    void SetErrorCallback(void (*callback)(int, const char*)) {
        glfwSetErrorCallback(callback);
    }

    void SetCursorPosCallback(void (*callback)(GLFWwindow*, double, double)) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(this->window, callback);
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(this->window);
    }

    float GetWidth() const {
        return settings.Width;
    }

    float GetHeight() const {
        return settings.Height;
    }

    void SetVSync(bool isVsync) {
        this->isVsync = isVsync;
        glfwSwapInterval(isVsync ? 1 : 0);
    }

    bool isVSync() const {
        return isVsync;
    }

    void Close() {
        glfwDestroyWindow(this->window);
    }

    operator GLFWwindow*() {
        return this->window;
    }
};

}
