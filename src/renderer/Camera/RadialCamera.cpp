#include "RadialCamera.h"

#include <glm/gtc/quaternion.hpp>

namespace GoL {

RadialCamera::RadialCamera(
        glm::vec3 center,
        glm::vec3 up,
        float radius,
        float width,
        float height,
        float phi,
        float theta,
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
    , Center(center)
    , WorldUp(up)
    , Phi(phi)
    , Theta(theta)
    , Radius(radius) {
    UpdateCameraVectors();
}

glm::mat4 RadialCamera::GetViewMatrix() const {
    return glm::lookAt(Position + Center, Center, Up);
}

glm::mat4 RadialCamera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(this->FoV), this->width / this->height, this->nearPlane, this->farPlane);
}

void RadialCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainTheta) {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    Phi -= xoffset;
    Theta -= yoffset;

    if (Phi > 360.0f) {
        Phi -= 360.0f;
    }
    if (Phi < -360.0f) {
        Phi += 360.0f;
    }
    if (constrainTheta) {
        if (Theta > 90.0f) {
            Theta = 89.9f;
        }
        if (Theta < -90.0f) {
            Theta = -89.9f;
        }
    }

    UpdateCameraVectors();
}

void RadialCamera::ProcessMouseScroll(float yoffset) {
    Radius -= yoffset * this->MouseSensitivity;
    FoV += (float) yoffset * this->MouseSensitivity;
    if (this->FoV < 1.0f) {
        this->FoV = 1.0f;
    }
    if (this->FoV > 75.0f) {
        this->FoV = 75.0f;
    }

    UpdateCameraVectors();
}

void RadialCamera::UpdateCameraVectors() {
    glm::vec3 Yaxis { 0.0f, 1.0f, 0.0f };

    glm::vec3 radiusVector { Radius, 0.0f, 0.0f };
    glm::quat rotationY = glm::angleAxis(glm::radians(Phi), Yaxis);

    radiusVector = rotationY * radiusVector;
    glm::vec3 perpendicularAxis = glm::normalize(glm::cross(radiusVector, Yaxis));

    glm::quat rotationUpDown = glm::angleAxis(glm::radians(Theta), perpendicularAxis);
    radiusVector = rotationUpDown * radiusVector;

    Position = radiusVector;
    Right = perpendicularAxis;
    Up = glm::normalize(glm::cross(Right, Position));
}

}
