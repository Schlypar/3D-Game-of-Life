#pragma once

// #include <imgui/imgui.h>
// #include <imgui/imgui_impl_glfw.h>
// #include <imgui/imgui_impl_opengl3.h>

#include "Camera.h"
#include "Window.h"

#include "IndexBuffer.h"

#include "Models/Cube.h"
#include "Models/Cursor3D.h"
#include "IndexBuffer.h"
#include "glad/gl.h"
#include "renderer/Renderer.h"
#include <string.h>
#include <iostream>

namespace GoL {

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

static GLuint CompileShaders() {
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
        //  "uniform mat4 ProjectionView;\n"
         "out vec4 i_color;\n"
         "void main()\n"
         "{\n"
         "  i_color = vec4(color.x, color.y, color.z, 0.0);\n"
         "  gl_Position = Matrix*vec4(Vertex.x, Vertex.y, Vertex.z, 1.0);\n"
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

    // glUseProgram(ShaderProgram);

    return ShaderProgram;
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
        window.Configure();
        window.SetErrorCallback(error_callback);
        window.SetCursorPosCallback(mouse_callback);

        // possibly other settings...
    }

    GLuint* setupExample() {
        int VERTEX_LOCATION = 0, MATRICES_LOCATION = 2;
        GLuint* retval = new GLuint[3];
        GLuint VBO, MBO, VAO;
        // float vertices[24] = {
        //     -0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
        //     0.3f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,
        //     0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f,
        //     -0.3f, 0.2f, 0.0f, 0.0f, 1.0f, 0.0f};
        
        // GLuint indices[6] = {
        //     1, 0, 2,
        //     3, 0, 2
        // };
        // int vert_size = 24 * sizeof(float);
        unsigned int indices[] = {
            0, 1,  // abscissa
            2, 3,  // ordinate
            4, 5   // applicata
        };
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

        int NUM_INSTANCES = 0;

        glm::mat4* mat = new glm::mat4[15000];
        for (float x = -20; x < 20; x += 1.5) {
            for (float y = -20; y < 20; y += 1.5) {
                for (float z = -20; z < 20; z += 1.5) {
                    mat[NUM_INSTANCES] = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3)), glm::vec3(x, y, z));
                    NUM_INSTANCES++;
                }
            }
        }
        std::cout << "num of instances: "<< NUM_INSTANCES << "\n";
        // mat[0] = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5)), glm::vec3(1.5, 0, 0));
        // mat[1] = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5)), glm::vec3(-1.5, 0, 0));
        glGenBuffers(1, &(MBO));
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 16 * sizeof(float), mat, GL_DYNAMIC_DRAW);
        for (unsigned int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(MATRICES_LOCATION + i);
            glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid*) (sizeof(GLfloat) * i * 4));
            glVertexAttribDivisorARB(MATRICES_LOCATION + i, 1);
        }

        retval[2] = IndexBuffer::Register(indices, 6);

        glBindVertexArray(0);

        // auto shader = CompileShaders();

        delete[] mat;

        retval[0] = VAO;
        // retval[1] = shader;
        retval[1] = NUM_INSTANCES;
        return retval;
    }

    void Run() {
        Renderer renderer;
        Cursor3DRenderer cursor_renderer;

        Shader shader = { "../resources/shaders/test.shader" };
        Shader instanceShader = { "../resources/shaders/instance.shader" };
        // shader.Bind();

        auto cube = Cube(glm::vec3 { 0.0f }, glm::vec3 { 0.0f }, 0.5f);
        cube.BindIndices();

        auto cursor = Cursor3D();
        cursor.BindIndices();

        // auto prism = Prism(glm::vec3 { 1.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 0.0f }, 0.5f);
        // prism.BindIndices();

        auto example_data = setupExample();

        IndexBuffer::Init();

        while (!window.ShouldClose()) {
            Parameters::currentFrameTime = static_cast<float>(glfwGetTime());
            Parameters::deltaTime = Parameters::currentFrameTime - Parameters::lastFrameTime;
            Parameters::lastFrameTime = Parameters::currentFrameTime;

            renderer.Clear();

            shader.Bind();
            renderer.Draw<Cube>(cube, Parameters::camera, shader);
            // // renderer.Draw<Prism>(prism, Parameters::camera, shader);
            // cursor.SetScaleFactor(100);
            // renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);
            // cursor.SetScaleFactor(0.1);
            // cursor_renderer.Draw<Cursor3D>(cursor, Parameters::camera, shader);

            // glUseProgram(example_data[1]);
            instanceShader.Bind();
            auto projMat = Parameters::camera.GetProjectionMatrix() * Parameters::camera.GetViewMatrix();
            instanceShader.SetUniformMat4f("ProjectionView", projMat);
            // glUniformMatrix4fv(glGetUniformLocation(example_data[1], "ProjectionView"), 1, GL_FALSE, &projMat[0][0]);
            glBindVertexArray(example_data[0]);
            // glDrawArraysInstanced(GL_TRIANGLES, 0, 3, (GLsizei) 2);
            glDrawElementsInstanced(
                // GL_TRIANGLES,
                GL_LINE_STRIP,
                IndexBuffer::GetCount(example_data[2]),
                GL_UNSIGNED_INT,
                IndexBuffer::GetOffset((example_data[2])),
                example_data[1]);
            glBindVertexArray(0);

            window.OnUpdate(Parameters::camera);
            ProcessInput();
        }
        delete[] example_data;
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