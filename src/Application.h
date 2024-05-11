#pragma once

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "InstanceHandler.h"
#include "Models/Vertex.h"
#include "Window.h"

#include "IndexBuffer.h"

#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "Models/Cursor3D_no_indices.h"
#include "Models/Prism.h"
#include "glad/gl.h"
#include "renderer/Renderer.h"

namespace GoL {

class Application {
public:
    struct Parameters {
        static bool firstMouse;
        static float lastX, lastY;

        static float currentFrameTime;
        static float lastFrameTime;
        static float deltaTime;

        static Camera camera;
    };

private:
    Window window;

public:
    Application(
            std::string title = "Game of Life",
            unsigned int width = 1980,
            unsigned int height = 1080
    )
        : window(Window::Settings(title, width, height)) {
    }

    void ConfigureCallbacks(
            void (*error_callback)(int, const char*),
            void (*mouse_callback)(GLFWwindow*, double, double)
    ) {
        // window settings
        window.SetVSync(true);
        window.Configure();
        window.SetErrorCallback(error_callback);
        window.SetCursorPosCallback(mouse_callback);

        // possibly other settings...
    }

    void Run() {

        Shader shader = { "../resources/shaders/test.shader" };
        Shader instanceShader = { "../resources/shaders/instance.shader" };
        shader.Bind();

        auto cube = Cube(glm::vec3 { 0.0f }, glm::vec3 { 0.0f }, 0.5f);
        int cube_ibo = cube.BindIndices();

        auto cursor = Cursor3D();
        cursor.BindIndices();
        auto cur = Cursor3D_no_i();
        auto cursors = InstanceHandler<Cursor3D_no_i>(cur);

        // InstanceHandler<Cube> cubes;
        std::vector<glm::mat4> instances;
        for (float i = -3; i < 3; i += 0.5) {
            for (float j = -3; j < 3; j += 0.5) {    
                glm::mat4 instanceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 0));
                instances.push_back(instanceMatrix);
            }
        }

        // auto prism = Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
        // prism.BindIndices();

        GLuint VBO, vao, modelVBO;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        auto v = cube.GetVerticies();
        glGenBuffers(1, &modelVBO);
        glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
        glBufferData(modelVBO, v.second * sizeof(Vertex), v.first, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), instances.data(), GL_DYNAMIC_DRAW);
        int matrix_layout_loc = 2;
        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(i + matrix_layout_loc);
            glVertexAttribPointer(i + matrix_layout_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
            glVertexAttribDivisorARB(i + matrix_layout_loc, 1);
        }

        glBindVertexArray(0);


        IndexBuffer::Init();

        while (!window.ShouldClose()) {
            Parameters::currentFrameTime = static_cast<float>(glfwGetTime());
            Parameters::deltaTime = Parameters::currentFrameTime - Parameters::lastFrameTime;
            Parameters::lastFrameTime = Parameters::currentFrameTime;

            Renderer::Clear();

            // Renderer::DrawBasic<Cube>(cube, Parameters::camera, shader);
            // Renderer::DrawBasic<Cursor3D_no_i>(cursos, Parameters::camera, shader);
            // // Renderer.Draw<Prism>(prism, Parameters::camera, shader);
            // cursor.SetScaleFactor(100);
            // Renderer::DrawBasic<Cursor3D>(cursor, Parameters::camera, shader);
            cursor.SetScaleFactor(0.1);
            Renderer::DrawCustom<Cursor3D, Cursor3D::CustomDraw>(cursor, Parameters::camera, shader);

            // Renderer::DrawInstanced(cursors, 0,  Parameters::camera, instanceShader);
            instanceShader.Bind();
            instanceShader.SetUniformMat4f("ProjectionView", Parameters::camera.GetProjectionMatrix() * Parameters::camera.GetViewMatrix());
            glBindVertexArray(vao);
            // glDrawArraysInstanced(GL_LINE, 0, 6, instances.size());
            glDrawElementsInstanced(GL_TRIANGLES, IndexBuffer::GetCount(cube_ibo), GL_UNSIGNED_INT, IndexBuffer::GetOffset(cube_ibo), instances.size());
            glBindVertexArray(0);

            window.OnUpdate(Parameters::camera);
            ProcessInput();
        }
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

private:
    void ProcessInput() {
        float cameraSpeed = static_cast<float>(2.5 * Parameters::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::LEFT, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::UP, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            Parameters::camera.ProcessKeyboard(GoL::CameraMovement::DOWN, Parameters::deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(0.0f, 10.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            Parameters::camera.ProcessMouseMovement(0.0f, -10.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, NULL);
            Parameters::firstMouse = false;
        }
        // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1 == GLFW_PRESS)) {
        //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //     glfwSetCursorPosCallback(window, mouse_callback);
        // }
    }
};

}