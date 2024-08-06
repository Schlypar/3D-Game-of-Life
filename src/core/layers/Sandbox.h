#pragma once

#include <list>

#include "Application.h"

#include "BatchRenderer.h"

#include "Camera/FlyingCamera.h"

// shaders
#include "shader_plain_color.h"

#include "Models/OneColorCube.h"
#include "Models/CubeFrame.h"

#include "MaterialLibrary.h"
#include "Materials/PlainColorMaterial.h"

#include "../Layer.h"
#include "imgui/imgui.h"
#include "layers/ImGuiLayer.h"

#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

namespace GoL {

class Sandbox : public Layer {
private:
    enum View { zeroes, first, second };

    FlyingCamera camera[2];
    BatchRenderer renderer;

    bool changed = false;

    bool firstMouse = true;
    bool mouseMoveHandle = false;
    float lastX = 0, lastY = 0;

    float currentFrameTime = 0;
    float lastFrameTime = 0;
    float deltaTime = 0;

    Shader plainColorShader;

    Model<Vertex>* cube;
    Model<Vertex>* selectedCube;
    Model<Vertex>* markedCube;

    unsigned int gridSize;
    float gridFactor = 0.25f;

    // coordinates here are meant in "cubes"
    const glm::vec3 nullCoords = { -1.0f, -1.0f, -1.0f };
    glm::vec3 selectedCoords = { -1.0f, -1.0f, -1.0f };
    std::list<glm::vec3> markedCoords;

    View view;

    glm::vec4 colorBase = { 0.2f, 0.3f, 0.6f, 1.0f };
    glm::vec4 colorSelected = { 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 colorMarked = { 0.855f, 0.09f, 0.14f, 1.0f };


public:
    Sandbox(
            std::string sceneName = "",
            int gridSize = 10,
            float width = 1920,
            float height = 1080)
        : Layer("Sandbox")
        , renderer()
        , plainColorShader(SHADER_PLAIN_COLOR)
        , gridSize(gridSize)
        , view(zeroes) {
            
        this->parentSceneName = sceneName;

        PlainColorMaterial* material = new PlainColorMaterial(plainColorShader, colorBase);
        MaterialLibrary::AddMaterial({ "PlainColorBase", material });

        material = new PlainColorMaterial(plainColorShader, colorSelected);
        MaterialLibrary::AddMaterial({ "PlainColorSelected", material });

        material = new PlainColorMaterial(plainColorShader, colorMarked);
        MaterialLibrary::AddMaterial({ "PlainColorMarked", material });
        
        cube = new CubeFrame(this->plainColorShader, glm::vec3{0.0f}, glm::vec3{0.0f});
        cube->SetScaleFactor(gridFactor * 0.95);
        cube->SetPosition({ 0.0f, 0.0f, 0.0f });
        cube->SetMaterial(MaterialLibrary::GetMaterial("PlainColorBase"));

        selectedCube = new CubeFrame(this->plainColorShader, glm::vec3{0.0f}, glm::vec3{0.0f});
        selectedCube->SetScaleFactor(gridFactor * 0.95);
        selectedCube->SetPosition({ 0.0f, 0.0f, 0.0f });
        selectedCube->SetMaterial(MaterialLibrary::GetMaterial("PlainColorSelected"));
        
        markedCube = new OneColorCube(this->plainColorShader, glm::vec3{0.0f}, glm::vec3{0.0f});
        markedCube->SetPosition({ 0.0f, 0.0f, 0.0f });
        markedCube->SetScaleFactor(gridFactor * 0.95);
        markedCube->SetMaterial(MaterialLibrary::GetMaterial("PlainColorMarked"));

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw = YAW;
        float pitch = PITCH;
        float nearPlane = 0.01f;
        float farPlane = 100.0;

        camera[0] = FlyingCamera(
            glm::vec3{ -gridFactor * gridSize / 2, gridFactor * gridSize, -1.5* gridFactor * gridSize },
            up, width, height, yaw - 30, pitch - 15, nearPlane, farPlane);
        camera[1] = FlyingCamera(
            glm::vec3{ gridFactor * gridSize / 2, 1.5 * gridFactor * gridSize, gridFactor * gridSize / 2 }
        );
    }

    ~Sandbox() = default;

    void OnAttach() override {
        this->OnChanged();

        Application& app = Application::Get();
        app.SubmitToImgui([this, &app]() {
            if (ImGui::Button("Done")) {
                // return configuration
                app.SwitchScene("main");
            };
        }, this->parentSceneName);
    }

    void OnDetach() override {
        delete cube;
        delete selectedCube;
        delete markedCube;
    }

    void OnUpdate() override {
        currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        renderer.Clear();
        if (changed) {
            renderer.Reset();
            this->OnChanged();
            changed = false;
        }

        renderer.DrawSubmitted(this->GetCurrentCamera());
    }

    void OnEvent(Event* e) override {
        EventDispatcher dispatcher(*e);

        dispatcher.Dispatch<KeyPressedEvent>(BIND_MEMBER_EVENT_FN(Sandbox::OnKeyPress));
        if (mouseMoveHandle) {
            dispatcher.Dispatch<MouseMovedEvent>(BIND_MEMBER_EVENT_FN(Sandbox::OnMouseMove));
        }
    }

    void SetMouseMoveHandler(bool mouseMove) {
        mouseMoveHandle = mouseMove;
    }

private:
    void OnChanged() {
        this->SubmitGrid();
        this->SubmitSelected();
        this->SubmitMarked();
    }

    const Camera3D* GetCurrentCamera() {
        if (view == second) {
            return &camera[1];
        }

        return &camera[0];
    }

    void SubmitGrid() {
        int xEnd, yEnd, zEnd, yStart = 0;
        xEnd = yEnd = zEnd = gridSize;

        if (view == second) {
            yStart = (int)selectedCoords.y;
            yEnd = yStart + 1;
        }

        for (int x = 0; x < xEnd; x++) {
            for (int y = yStart; y < yEnd; y++) {
                if (view == first && selectedCoords.y == y) {
                    continue;
                }
                for (int z = 0; z < zEnd; z++) {
                    if (selectedCoords.x == x && selectedCoords.z == z) {
                        continue;
                    }
                    cube->SetPosition({ x * gridFactor, y * gridFactor, z * gridFactor });
                    renderer.Submit(cube);
                }
            }
        }
    }

    void SubmitSelected() {
        if (selectedCoords == nullCoords) {
            return;
        }

        if (view == second) {
            if (selectedCoords.x == -1 || selectedCoords.z == -1 || selectedCoords.y == -1) {
                return;
            }
            selectedCube->SetPosition({
                selectedCoords.x * gridFactor,
                selectedCoords.y * gridFactor,
                selectedCoords.z * gridFactor
            });
            renderer.Submit(selectedCube);
            return;
        }

        int xStart, xEnd, yStart, yEnd, zStart, zEnd;

        if (selectedCoords.x < 0) {
            xStart = 0;
            xEnd = gridSize;
        } else {
            xStart = selectedCoords.x;
            xEnd = xStart + 1;
        }
        if (selectedCoords.y < 0) {
            yStart = 0;
            yEnd = gridSize;
        } else {
            yStart = selectedCoords.y;
            yEnd = yStart + 1;
        }
        if (selectedCoords.z < 0) {
            zStart = 0;
            zEnd = gridSize;
        } else {
            zStart = selectedCoords.z;
            zEnd = zStart + 1;
        }

        for (int x = xStart; x < xEnd; x++) {
            for (int y = yStart; y < yEnd; y++) {
                for (int z = zStart; z < zEnd; z++) {
                    selectedCube->SetPosition({ x * gridFactor, y * gridFactor, z * gridFactor });
                    renderer.Submit(selectedCube);
                }
            }
        }
    }

    void SubmitMarked() {
        for (auto& markedPos : markedCoords) {
            if (view == second && markedPos.y != selectedCoords.y) {
                continue;
            }

            markedCube->SetPosition(markedPos * gridFactor);
            renderer.Submit(markedCube);
        }
    }

    void UpdateCameraOnView() {
        camera[1] = FlyingCamera(
            glm::vec3{ gridFactor * gridSize / 2, 20 * gridFactor + gridFactor * selectedCoords.y, gridFactor * gridSize / 2 },
            glm::vec3{0.0f, 0.0f, 1.0f},
            camera[0].width,
            camera[0].height,
            0.0f,
            -90.0f,
            camera[0].nearPlane,
            camera[0].farPlane
        );
    }

    bool OnKeyPress(KeyPressedEvent& e) {
        auto key = e.GetKeyCode();
        switch (view) {
        case zeroes:
            switch (key) {
            case GLFW_KEY_ENTER:
                view = first;
                selectedCoords.y = 0;
                changed = true;
                break;
            case GLFW_KEY_ESCAPE:
                break;
                // exit sandbox
            }
            break;
        case first:
            switch (key) {
            case GLFW_KEY_ENTER:
                if (selectedCoords != nullCoords) {
                    selectedCoords.x = gridSize / 2;
                    selectedCoords.z = gridSize / 2;
                    view = second;
                    this->UpdateCameraOnView();
                    changed = true;
                }
                break;
            case GLFW_KEY_ESCAPE:
                view = zeroes;
                selectedCoords = nullCoords;
                changed = true;
                break;
            case GLFW_KEY_UP:
                if (selectedCoords.y < 0) {
                    selectedCoords.y = 0;
                    changed = true;
                } else if (selectedCoords.y == gridSize - 1) {
                    selectedCoords.y = 0;
                    changed = true;
                } else if (selectedCoords.y < gridSize) {
                    selectedCoords.y++;
                    changed = true;
                }
                break;
            case GLFW_KEY_DOWN:
                if (selectedCoords.y <= 0) {
                    selectedCoords.y = gridSize - 1;
                    changed = true;
                } else if (selectedCoords.y > 0) {
                    selectedCoords.y--;
                    changed = true;
                }
                break;
            default:
                break;
            }
            break;
        case second:
            switch (key) {
            case GLFW_KEY_ENTER:
                this->ClickOnSelected();
                changed = true;
                break;
            case GLFW_KEY_ESCAPE:
                view = first;
                selectedCoords.x = -1;
                selectedCoords.z = -1;
                changed = true;
                break;
            case GLFW_KEY_UP:
                if (selectedCoords.z < 0) {
                    selectedCoords.z = 0;
                    changed = true;
                } else if (selectedCoords.z == gridSize - 1) {
                    selectedCoords.z = 0;
                    changed = true;
                } else if (selectedCoords.z < gridSize) {
                    selectedCoords.z++;
                    changed = true;
                }
                break;
            case GLFW_KEY_DOWN:
                if (selectedCoords.z <= 0) {
                    selectedCoords.z = gridSize - 1;
                    changed = true;
                } else if (selectedCoords.z > 0) {
                    selectedCoords.z--;
                    changed = true;
                }
                break;
            case GLFW_KEY_LEFT:
                if (selectedCoords.x < 0) {
                    selectedCoords.x = 0;
                    changed = true;
                }  else if (selectedCoords.x == gridSize - 1) {
                    selectedCoords.x = 0;
                    changed = true;
                } else if (selectedCoords.x < gridSize) {
                    selectedCoords.x++;
                    changed = true;
                }
                break;
            case GLFW_KEY_RIGHT:
                if (selectedCoords.x <= 0) {
                    selectedCoords.x = gridSize - 1;
                    changed = true;
                } else if (selectedCoords.x > 0) {
                    selectedCoords.x--;
                    changed = true;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
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

        camera[0].ProcessMouseMovement(offset.x, offset.y);

        return true;
    }

    void ClickOnSelected() {
        if (!markedCoords.remove(selectedCoords)) {
            markedCoords.push_back(selectedCoords);
        }
    };
};
}
