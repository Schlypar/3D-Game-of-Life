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
    using EventCallbackFn = std::function<void(Event*)>;

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

    /**
    * @name Configure - Toggles OpenGL state settings
    * @return void
    */
    void Configure();

    /**
    * @name OnUpdate - Updates Window every frame
    * @return void
    */
    void OnUpdate();

    /**
    * @name SetEventCallback - Customizes behaviour of window on events
    * @param callback -  function to be called to handle events
    * @return void
    */
    void SetEventCallback(const EventCallbackFn& callback);

    /**
    * @name ShouldClose - Tells whether or not Window should be closed
    * @return bool
    */
    inline bool ShouldClose() const {
        return glfwWindowShouldClose(this->window);
    }

    /**
    * @name GetWidth - Returns width of Window in pixels
    * @return float - pixels
    */
    inline float GetWidth() const {
        return data.Width;
    }

    /**
    * @name GetHeight - Returns height of Window in pixels
    * @return float - pixels
    */
    inline float GetHeight() const {
        return data.Height;
    }

    /**
    * @name SetVSync - Sets VSync to boolean
    * @param isVsync - Flag is Vsync
    * @return void
    */
    inline void SetVSync(bool isVsync) {
        this->isVsync = isVsync;
        glfwSwapInterval(isVsync ? 1 : 0);
    }

    /**
    * @name isVSync - Status of isVsync
    * @return bool
    */
    inline bool isVSync() const {
        return isVsync;
    }

    /**
    * @name Close - Closes the Window
    * @return void
    */
    inline void Close() {
        glfwDestroyWindow(this->window);
    }

    inline operator GLFWwindow*() {
        return this->window;
    }
};

}
