#pragma once

#include "Models/OneColorCube.h"
#include "Models/Prism.h"
#include "Renderer.h"

#include "Models/SixColorCube.h"

#include "../Layer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace GoL {

class MainLayer : public Layer {
private:
    Camera camera;
    Renderer renderer;

    bool firstMouse = true;
    float lastX = 0, lastY = 0;

    float currentFrameTime = 0;
    float lastFrameTime = 0;
    float deltaTime = 0;

    Shader prismShader;
    Shader cubeShader;
    Model* model1;
    Model* model2;

public:
    MainLayer(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, -3.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float width = 1920,
            float height = 1080,
            float yaw = YAW,
            float pitch = PITCH,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
    )
        : Layer("Game of Life layer")
        , camera(position, up, width, height, yaw, pitch, nearPlane, farPlane)
        , renderer()
        , prismShader("resources/shaders/prism.shader")
        , cubeShader("resources/shaders/plain_color.shader") {
        model1 = new SixColorCube(this->cubeShader);
        model1->SetScaleFactor(0.5f);
        model1->SetPosition({ -0.25, -0.75f, 0.1f });
        model1->SetRotation(glm::vec3 { 15.0f });
        // model2 = new OneColorCube(this->cubeShader);
        // // model2->SetPosition({ 0.5f, 0.25f, 0.0f });
        // model2->SetScaleFactor(0.5f);
        // // model2->SetPosition({ 0.45, -0.45f, -0.1f });
    }

    ~MainLayer() = default;

    void OnAttach() override {
    }

    void OnDetach() override {
        delete model1;
        // delete model2;
    }

    void OnUpdate() override {
        currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        std::cout << "RESET\n";
        renderer.Clear();

        renderer.Submit(model1);
        // renderer.Submit(model2);
        renderer.DrawSubmitted(camera);
        // renderer.Draw(model1, camera);
        // renderer.Draw(model2, camera);
        // renderer.Draw(model3, camera);
        // renderer.Draw(model4, camera);
    }

    void OnEvent(Event& e) override {
        EventDispatcher dispatcher(e);

        // probably make it async in the future ?
        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(MainLayer::OnKeyPress));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_MEMBER_EVENT_FN(MainLayer::OnMouseMove));
    }

private:
    bool OnKeyPress(KeyPressedEvent& e) {
        if (e.GetKeyCode() == GLFW_KEY_W) {
            camera.ProcessKeyboard(GoL::CameraMovement::FORWARD, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_S) {
            camera.ProcessKeyboard(GoL::CameraMovement::BACKWARD, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_A) {
            camera.ProcessKeyboard(GoL::CameraMovement::LEFT, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_D) {
            camera.ProcessKeyboard(GoL::CameraMovement::RIGHT, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT_SHIFT) {
            camera.ProcessKeyboard(GoL::CameraMovement::UP, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT_CONTROL) {
            camera.ProcessKeyboard(GoL::CameraMovement::DOWN, deltaTime);
        }
        if (e.GetKeyCode() == GLFW_KEY_Q) {
            camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_E) {
            camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_LEFT) {
            camera.ProcessMouseMovement(-10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_RIGHT) {
            camera.ProcessMouseMovement(10.0f, 0.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_UP) {
            camera.ProcessMouseMovement(0.0f, 10.0f);
        }
        if (e.GetKeyCode() == GLFW_KEY_DOWN) {
            camera.ProcessMouseMovement(0.0f, -10.0f);
        }

        return true;
    }

    bool OnMouseMove(MouseMovedEvent& e) {
        if (firstMouse) {
            lastX = e.GetX();
            lastY = e.GetY();
            firstMouse = false;
        }

        glm::vec2 offset(e.GetX() - lastX, lastY - e.GetY());
        lastX = e.GetX();
        lastY = e.GetY();

        camera.ProcessMouseMovement(offset.x, offset.y);

        return true;
    }
};
}
