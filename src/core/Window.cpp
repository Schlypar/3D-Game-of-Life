#include "Window.h"

#include "events/ApplicationEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace GoL {

Window::Window(const Data& settings) {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    data.Title = settings.Title;
    data.Width = settings.Width;
    data.Height = settings.Height;

    glfwSetErrorCallback(error_callback);
    this->window = glfwCreateWindow(
            settings.Width,
            settings.Height,
            settings.Title.c_str(),
            NULL,
            NULL
    );
    if (!this->window) {
        CORE_ERROR("Unable to initialize window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    gladLoadGL(glfwGetProcAddress);
    glfwSetWindowUserPointer(this->window, &this->data);

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        auto event = new WindowResizeEvent(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);
        auto event = new WindowCloseEvent;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS:
                {
                    auto event = new KeyPressedEvent(key, 0);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    auto event = new KeyReleasedEvent(key);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    auto event = new KeyPressedEvent(key, true);
                    data.EventCallback(event);
                    break;
                }
        }
    });

    glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);

        auto event = new KeyTypedEvent(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);

        switch (action) {
            case GLFW_PRESS:
                {
                    auto event = new MouseButtonPressedEvent(button);
                    data.EventCallback(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    auto event = new MouseButtonReleasedEvent(button);
                    data.EventCallback(event);
                    break;
                }
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);

        auto event = new MouseScrolledEvent((float) xOffset, (float) yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
        Data& data = *(Data*) glfwGetWindowUserPointer(window);

        auto event = new MouseMovedEvent((float) xPos, (float) yPos);
        data.EventCallback(event);
    });
}

Window::~Window() {
    Close();
}

void Window::Configure() {
    glEnable(GL_DEPTH_TEST);
    OPENGL_INFO("Enabled GL_DEPTH_TEST");

    glEnable(GL_CULL_FACE);
    OPENGL_INFO("Enabled GL_CULL_FACE");
    glFrontFace(GL_CCW);
    OPENGL_INFO("Front face is CCW");
    glCullFace(GL_BACK);
    OPENGL_INFO("Back face is culled");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    OPENGL_INFO("glPolygonMode: GL_FRONT_AND_BACK, GL_FILL");
}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(this->window);
}

void Window::SetEventCallback(const EventCallbackFn& callback) {
    this->data.EventCallback = callback;
}

}
