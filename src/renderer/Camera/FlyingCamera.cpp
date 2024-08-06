#include "FlyingCamera.h"
#include "Camera/Camera3D.h"

namespace GoL {

FlyingCamera::FlyingCamera(
        glm::vec3 position,
        glm::vec3 up,
        float width,
        float height,
        float yaw,
        float pitch,
        float nearPlane,
        float farPlane
)
    : Camera3D(
              SPEED,
              SENSITIVITY,
              ZOOM,
              width,
              height,
              nearPlane,
              farPlane
      )
    , Front(glm::vec3(0.0f, 0.0f, -1.0f)) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 FlyingCamera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 FlyingCamera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(this->FoV), this->width / this->height, this->nearPlane, this->farPlane);
}

void FlyingCamera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }
    if (direction == UP) {
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
    }
    if (direction == DOWN) {
        Position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
    }
}

void FlyingCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void FlyingCamera::ProcessMouseScroll(float yoffset) {
    FoV -= (float) yoffset;
    if (this->FoV < 1.0f) {
        this->FoV = 1.0f;
    }
    if (this->FoV > 75.0f) {
        this->FoV = 75.0f;
    }
}

void FlyingCamera::UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    this->Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(Right, Front));
}

}
