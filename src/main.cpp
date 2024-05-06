#include <cstddef>
#include <stdio.h>
#include <stdlib.h>

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "Window.h"

#include "Camera.h"
#include "IndexBuffer.h"
#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "Models/Prism.h"
#include "renderer/Renderer.h"

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main(void) {
    GoL::Window window = GoL::Window(GoL::Window::Settings());
    window.SetVSync(true);
    window.Configure();

    GoL::Shader shader = { "resources/shaders/test.shader" };
    shader.Bind();

    GoL::Camera camera = GoL::Camera({ 0.0f, 0.0f, 3.0f }, { 0.0f, 1.0f, 0.0f }, window.GetWidth(), window.GetHeight());
    GoL::cam = &camera;
    GoL::Renderer renderer;
    GoL::Cursor3DRenderer cursor_renderer;

    auto cube = GoL::Cube(glm::vec3 { 0.0f }, glm::vec3 { 15.0f, 0.0f, 45.0f }, 0.5f);
    cube.BindIndices();

    auto cursor = GoL::Cursor3D();
    cursor.BindIndices();

    auto prism = GoL::Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
    prism.BindIndices();

    GoL::IndexBuffer::Init();

    while (!window.ShouldClose()) {
        renderer.Clear();

        renderer.Draw<GoL::Cube>(cube, camera, shader);
        renderer.Draw<GoL::Prism>(prism, camera, shader);
        cursor.SetScaleFactor(100);
        renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);
        cursor.SetScaleFactor(0.1);
        cursor_renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);

        window.OnUpdate(camera);
    }

    // while (!glfwWindowShouldClose(window)) {
    //     float currentFrame = static_cast<float>(glfwGetTime());
    //     deltaTime = currentFrame - lastFrame;
    //     lastFrame = currentFrame;

    //     processInput(window, camera);

    //     renderer.Clear();

    //     renderer.Draw<GoL::Cube>(cube, camera, shader);
    //     renderer.Draw<GoL::Prism>(prism, camera, shader);
    //     cursor.SetScaleFactor(100);
    //     renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);
    //     cursor.SetScaleFactor(0.1);
    //     cursor_renderer.Draw<GoL::Cursor3D>(cursor, camera, shader);

    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }

    // window.Close();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
