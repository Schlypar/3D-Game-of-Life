#pragma once

#include "Application.h"
#include "BatchRenderer.h"
#include "RandomRenderer.h"

// shaders
#include "shader_plain_color.h"
#include "shader_prism.h"

#include "Models/OneColorCube.h"
#include "Models/SixColorCube.h"

#include "MaterialLibrary.h"
#include "Materials/PlainColorMaterial.h"

#include "../Layer.h"
#include "imgui/imgui.h"
#include "layers/ImGuiLayer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace GoL {

class MainLayer : public Layer {
private:
    Camera camera;
    BatchRenderer batchRenderer;
    RandomRenderer randomRenderer;

    bool firstMouse = true;
    bool mouseMoveHandle = true;
    float lastX = 0, lastY = 0;

    float currentFrameTime = 0;
    float lastFrameTime = 0;
    float deltaTime = 0;

    Shader prismShader;
    Shader cubeShader;
    Model<Vertex>* sixColor;
    Model<Vertex>* oneColor;

    glm::vec4 color = { 0.3f, 0.4f, 0.7f, 1.0f };

public:
    MainLayer(
            std::string sceneName = "",
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
        , batchRenderer()
        , prismShader(SHADER_PRISM)
        , cubeShader(SHADER_PLAIN_COLOR) {
        this->parentSceneName = sceneName;

        sixColor = new SixColorCube(this->cubeShader);
        sixColor->SetScaleFactor(0.35f);
        sixColor->SetPosition({ -0.5f, 0.25f, 0.25f });
        sixColor->SetRotation(glm::vec3 { 15.0f });

        oneColor = new OneColorCube(this->cubeShader);
        oneColor->SetPosition({ 0.5f, -0.35f, -0.25f });
        oneColor->SetScaleFactor(0.05f);
    }

    ~MainLayer() = default;

    void OnAttach() override {
        PlainColorMaterial* mat = new PlainColorMaterial(cubeShader, color);
        MaterialLibrary::AddMaterial({ "SharedPlainColor", mat });
        // oneColor->SetMaterial(MaterialLibrary::GetMaterial("SharedPlainColor"));
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                for (int z = 0; z < 10; z++) {
                    oneColor->SetPosition({ x * 0.25f, y * 0.25f, z * 0.25f });
                    oneColor->SetRotation(glm::vec3 { x * 10, y * 10, z * 10 });
                    // batchRenderer.Submit(oneColor);
                    randomRenderer.Submit(oneColor);
                }
            }
        }

        Application& app = Application::Get();
        app.SubmitToImgui([this, &app]() {
            ImGui::InputFloat("RED", &color.r);
            ImGui::InputFloat("GREEN", &color.g);
            ImGui::InputFloat("BLUE", &color.b);
            ImGui::InputFloat("ALPHA", &color.a);

            auto mat = (PlainColorMaterial*) MaterialLibrary::GetMaterial("SharedPlainColor").material;
            mat->SetColor(color);

            if (ImGui::Button("Resubmit")) {
                this->batchRenderer.Reset();
                for (int x = 0; x < 10; x++) {
                    for (int y = 0; y < 10; y++) {
                        for (int z = 0; z < 10; z++) {
                            oneColor->SetPosition({ x * 0.25f, y * 0.25f, z * 0.25f });
                            oneColor->SetRotation(glm::vec3 { x * 10, y * 10, z * 10 });
                            batchRenderer.Submit(oneColor);
                        }
                    }
                }
            };

            if (ImGui::Button("Sandbox")) {
                app.SwitchScene("sandbox");
            }
        },
                          this->parentSceneName);
    }

    void OnDetach() override {
        delete sixColor;
        delete oneColor;
    }

    void OnUpdate() override {
        currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // batchRenderer.Clear();
        // batchRenderer.DrawSubmitted(camera);

        randomRenderer.Clear();
        randomRenderer.DrawSubmitted(camera);
    }

    void OnEvent(Event* e) override {
        EventDispatcher dispatcher(*e);

        // probably make it async in the future ?
        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(MainLayer::OnKeyPress));
        if (mouseMoveHandle) {
            dispatcher.Dispatch<MouseMovedEvent>(BIND_MEMBER_EVENT_FN(MainLayer::OnMouseMove));
        }
    }

    void SetMouseMoveHandler(bool mouseMove) {
        mouseMoveHandle = mouseMove;
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
