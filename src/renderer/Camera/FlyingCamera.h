#pragma once

#include "Camera3D.h"

namespace GoL {

constexpr float YAW = 90.0f;
constexpr float PITCH = 0.0f;

class FlyingCamera : public Camera3D {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    FlyingCamera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float width = 4,
            float height = 3,
            float yaw = YAW,
            float pitch = PITCH,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
    );

    glm::mat4 GetViewMatrix() const override;
    glm::mat4 GetProjectionMatrix() const override;

    /**
    * @name ProcessKeyboard - Moves Camera in coordinate space of observer (camera)
    * @param direction -  Direction to move
    * @param deltaTime - Value of delta Time between frames
    * @return void
    */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    /**
    * @name ProcessMouseMovement - Processes the change of perspective by mouse movement
    * @param xoffset - Value of mouse xoffset
    * @param yoffset - Value of mouse yoffset
    * @param constrainPitch - Whether to constrain angles to 90 degree max
    * @return void
    */
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    /**
    * @name ProcessMouseScroll - Processes zoom
    * @param yoffset - Value of movement
    * @return void
    */
    void ProcessMouseScroll(float yoffset);

private:
    void UpdateCameraVectors();
};

}
