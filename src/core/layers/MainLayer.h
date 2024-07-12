#pragma once

#include "Application.h"
#include "Batcher.h"
#include "MaterialLibrary.h"
#include "Materials/PlainColorMaterial.h"
#include "Models/OneColorCube.h"
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
    Model<Vertex>* sixColor;
    Model<Vertex>* oneColor;

    Batcher batcher = Batcher(Batcher::Config {});
    std::vector<Surface<Vertex>> batches;

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
        sixColor = new SixColorCube(this->cubeShader);
        sixColor->SetScaleFactor(0.25f);
        sixColor->SetRotation(glm::vec3 { 15.0f });
        oneColor = new OneColorCube(this->cubeShader);
        oneColor->SetScaleFactor(0.05f);
    }

    ~MainLayer() = default;

    void OnAttach() override {
        Application& app = Application::Get();
        app.SubmitToImgui([this]() {
            if (ImGui::Button("Press me")) {
                // this->batcher.Reset();
                // this->batches = std::move(this->batcher.ComputeBatches());
                this->renderer.ResetBatched();
                this->renderer.ConcatenateGeometry();
            }
        });
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                for (int z = 0; z < 10; z++) {
                    oneColor->SetPosition({ x * 0.25f, y * 0.25f, z * 0.25f });
                    oneColor->SetRotation(glm::vec3 { x * 10, y * 10, z * 10 });
                    renderer.Submit(oneColor);
                }
            }
        }
        renderer.ConcatenateGeometry();

        // for (int x = 0; x < 10; x++) {
        //     for (int y = 0; y < 10; y++) {
        //         for (int z = 0; z < 10; z++) {
        //             oneColor->SetPosition({ x * 0.25f, y * 0.25f, z * 0.25f });
        //             oneColor->SetRotation(glm::vec3 { x * 10, y * 10, z * 10 });
        //             // renderer.Submit(oneColor);
        //             batcher.Submit(oneColor);
        //         }
        //     }
        // }
        // this->batches = batcher.ComputeBatches();
    }

    void OnDetach() override {
        delete sixColor;
        delete oneColor;
    }

    void OnUpdate() override {
        currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        renderer.Clear();

        // renderer.DrawTest(this->batches, camera);
        renderer.DrawSubmitted(camera);
        // batcher.Reset();
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
