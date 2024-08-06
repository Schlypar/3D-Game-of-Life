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
    : Center(center)
    , MovementSpeed(SPEED2)
    , MouseSensitivity(SENSITIVITY2)
    , FoV(ZOOM2)
    , width(width)
    , height(height)
    , nearPlane(nearPlane)
    , farPlane(farPlane) {
    Radius = radius;
    WorldUp = up;
    Phi = phi;
    Theta = theta;
    UpdateCameraVectors();
}

glm::mat4 RadialCamera::GetViewMatrix() const {
    return glm::lookAt(Position + Center, Center, Up);
}

glm::mat4 RadialCamera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(FoV), width / height, nearPlane, farPlane);
}

void RadialCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainTheta) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

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
            Theta = 90.0f;
        }
        if (Theta < -90.0f) {
            Theta = -90.0f;
        }
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void RadialCamera::ProcessMouseScroll(float yoffset) {
    Radius += yoffset * MouseSensitivity;
    FoV -= (float) yoffset * MouseSensitivity;
    if (FoV < 1.0f) {
        FoV = 1.0f;
    }
    if (FoV > 75.0f) {
        FoV = 75.0f;
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
