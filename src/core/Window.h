#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "events/Event.h"

#include <functional>
#include <string>

namespace GoL {

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

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
    Window(const Data& settings);
    ~Window();

    void Configure();

    void OnUpdate();

    void SetEventCallback(const EventCallbackFn& callback);

    inline bool ShouldClose() const {
        return glfwWindowShouldClose(this->window);
    }

    inline float GetWidth() const {
        return data.Width;
    }

    inline float GetHeight() const {
        return data.Height;
    }

    inline void SetVSync(bool isVsync) {
        this->isVsync = isVsync;
        glfwSwapInterval(isVsync ? 1 : 0);
    }

    inline bool isVSync() const {
        return isVsync;
    }

    inline void Close() {
        glfwDestroyWindow(this->window);
    }

    inline operator GLFWwindow*() {
        return this->window;
    }
};

}
