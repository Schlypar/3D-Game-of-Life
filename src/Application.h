#pragma once

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

// #include "InstanceHandler.h"
// #include "Models/Vertex.h"
// #include "VertexBuffer.h"
// #include "VertexBufferLayout.h"
#include "Shader.h"
#include "Window.h"

#include "IndexBuffer.h"

// #include "Models/Cube.h"
// #include "Models/Cursor3D.h"
#include "Models/Cursor3D_no_indices.h"
// #include "Models/Prism.h"
#include "glad/gl.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "renderer/Renderer.h"
#include <cstring>

namespace GoL {

#define VERTEX_LOCATION 0

// static GLuint VBO;
// static GLuint MBO;
#define MATRICES_LOCATION 2

// static void CreateVertexBuffer() {
//     float vertices[9] = { -0.3f, -0.3f, 0.0f, 0.3f, -0.3f, 0.0f, 0.0f, 0.25f, 0.0f };
//     glGenBuffers(1, &VBO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
//     glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, (GLsizei) 3 * sizeof(float), NULL);
//     glEnableVertexAttribArray(VERTEX_LOCATION);

//     float mat[NUM_INSTANCES * 16];
//     for (int matrix_id = 0; matrix_id < NUM_INSTANCES; matrix_id++) {
//         float pos_x = 0.002f * matrix_id * cos(40 * M_PI * matrix_id / NUM_INSTANCES);
//         float pos_y = 0.002f * matrix_id * sin(40 * M_PI * matrix_id / NUM_INSTANCES);
//         float scale = 0.0004f * matrix_id;
//         int i = 16 * matrix_id;
//         mat[i + 0] = scale;
//         mat[i + 4] = 0.0f;
//         mat[i + 8] = 0.0f;
//         mat[i + 12] = pos_x;
//         mat[i + 1] = 0.0f;
//         mat[i + 5] = scale;
//         mat[i + 9] = 0.0f;
//         mat[i + 13] = pos_y;
//         mat[i + 2] = 0.0f;
//         mat[i + 6] = 0.0f;
//         mat[i + 10] = scale;
//         mat[i + 14] = 0.0f;
//         mat[i + 3] = 0.0f;
//         mat[i + 7] = 0.0f;
//         mat[i + 11] = 0.0f;
//         mat[i + 15] = 1.0f;
//     }
//     glGenBuffers(1, &(MBO));
//     glBindBuffer(GL_ARRAY_BUFFER, MBO);
//     glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 16 * sizeof(float), mat, GL_DYNAMIC_DRAW);
//     for (unsigned int i = 0; i < 4; i++) {
//         glEnableVertexAttribArray(MATRICES_LOCATION + i);
//         glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid*) (sizeof(GLfloat) * i * 4));
//         glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
//     }
// }

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) {
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint) strlen(pShaderText);

    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders() {
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    vs = "#version 330 core\n"
         "layout (location = 0) in vec3 Vertex;\n"
         "layout (location = 1) in vec3 color;"
         "layout (location = 2) in mat4 Matrix;\n"
         "uniform mat4 ProjectionView;\n"
         "out vec4 i_color;\n"
         "void main()\n"
         "{\n"
         "  i_color = vec4(color.x, color.y, color.z, 0.0);\n"
         "  gl_Position = ProjectionView*Matrix*vec4(Vertex.x, Vertex.y, Vertex.z, 1.0);\n"
         "}\n";
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    fs = "#version 330 core\n"
         "in vec4 i_color;\n"
         "out vec4 FragColor;\n"
         "void main()\n"
         "{\n"
         "  FragColor = i_color;\n"
         "}\n";
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);
}

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
        // window.Configure();
        window.SetErrorCallback(error_callback);
        window.SetCursorPosCallback(mouse_callback);

        // possibly other settings...
    }

    GLuint setupExample() {
        GLuint VBO, MBO, VAO;
        // float vertices[18] = {
        //     -0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
        //     0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
        //     0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,};
            
        float vertices[36] = {
            0.0f, 0.0f, 0.0f, 1, 0, 0, // center
            1.0f, 0.0f, 0.0f, 1, 0, 0, // abscissa
            0.0f, 0.0f, 0.0f, 0, 1, 0, // center
            0.0f, 1.0f, 0.0f, 0, 1, 0, // ordinate
            0.0f, 0.0f, 0.0f, 0, 0, 1, // center
            0.0f, 0.0f, 1.0f, 0, 0, 1  // applicata
        };
        int vert_size = 36 * sizeof(float);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert_size, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(VERTEX_LOCATION, 3, GL_FLOAT, GL_FALSE, (GLsizei) 6 * sizeof(float), NULL);
        glEnableVertexAttribArray(VERTEX_LOCATION);
        glVertexAttribPointer(VERTEX_LOCATION + 1, 3, GL_FLOAT, GL_FALSE, (GLsizei) 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(VERTEX_LOCATION + 1);

        int NUM_INSTANCES = 2;
        // float mat[NUM_INSTANCES * 16];
        // for (int matrix_id = 0; matrix_id < NUM_INSTANCES; matrix_id++) {
        //     float pos_x = 0.002f * matrix_id * cos(40 * M_PI * matrix_id / NUM_INSTANCES);
        //     float pos_y = 0.002f * matrix_id * sin(40 * M_PI * matrix_id / NUM_INSTANCES);
        //     float scale = 0.0004f * matrix_id;
        //     int i = 16 * matrix_id;
        //     mat[i + 0] = scale;
        //     mat[i + 4] = 0.0f;
        //     mat[i + 8] = 0.0f;
        //     mat[i + 12] = pos_x;
        //     mat[i + 1] = 0.0f;
        //     mat[i + 5] = scale;
        //     mat[i + 9] = 0.0f;
        //     mat[i + 13] = pos_y;
        //     mat[i + 2] = 0.0f;
        //     mat[i + 6] = 0.0f;
        //     mat[i + 10] = scale;
        //     mat[i + 14] = 0.0f;
        //     mat[i + 3] = 0.0f;
        //     mat[i + 7] = 0.0f;
        //     mat[i + 11] = 0.0f;
        //     mat[i + 15] = 1.0f;
        // }
        glm::mat4* mat = new glm::mat4[2];
        mat[0] = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0, 0));
        mat[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, 0, 0));
        glGenBuffers(1, &(MBO));
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 16 * sizeof(float), mat, GL_DYNAMIC_DRAW);
        for (unsigned int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(MATRICES_LOCATION + i);
            glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid*) (sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
        }

        glBindVertexArray(0);

        CompileShaders();

        return VAO;
    }

    void Run() {
        GLclampf Red = 0.2f, Green = 0.2f, Blue = 0.2f, Alpha = 0.0f;
        glClearColor(Red, Green, Blue, Alpha);

        // CreateVertexBuffer();
        auto VAO_example = setupExample();

        
        // Shader shader = { "resources/shaders/instance.shader" };
        // shader.Bind();
        // shader.SetUniformMat4f("ProjectionView", glm::mat4(1.0));

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);

            glUniformMatrix4fv(GetUniformLocation("ProjectionView"), 1, GL_FALSE, &matrix[0][0]);
            glBindVertexArray(VAO_example);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 3, (GLsizei) 2);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
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