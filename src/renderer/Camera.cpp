#include "Camera.h"

namespace GoL {

Camera::Camera(glm::vec3 position, glm::vec3 up, float width, float height, float yaw, float pitch, float nearPlane, float farPlane)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    , MovementSpeed(SPEED)
    , MouseSensitivity(SENSITIVITY)
    , FoV(ZOOM)
    , width(width)
    , height(height)
    , nearPlane(nearPlane)
    , farPlane(farPlane) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(FoV), width / height, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
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

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

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

void Camera::ProcessMouseScroll(float yoffset) {
    FoV -= (float) yoffset;
    if (FoV < 1.0f) {
        FoV = 1.0f;
    }
    if (FoV > 75.0f) {
        FoV = 75.0f;
    }
}

void Camera::UpdateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

}
