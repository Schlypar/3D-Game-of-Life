#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Camera.h"
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <string>

namespace GoL {

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

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

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
