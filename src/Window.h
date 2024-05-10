#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "events/Event.h"

#include <functional>
#include <string>

namespace GoL {

class Window {
protected:
    using EventCallbackFn = std::function<void(Event&)>;

public:
    struct Data {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        EventCallbackFn EventCallback;

        Data(
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
    Data data;
    bool isVsync;

public:
    Window(const Data& settings) {
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
        this->data = settings;
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

    void SetEventCallback(const EventCallbackFn& callback) {
        this->data.EventCallback = callback;
    }

    bool ShouldClose() const {
        return glfwWindowShouldClose(this->window);
    }

    float GetWidth() const {
        return data.Width;
    }

    float GetHeight() const {
        return data.Height;
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
