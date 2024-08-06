#ifndef RADIALCAMERA_H_
#define RADIALCAMERA_H_

#include "Camera3D.h"

namespace GoL {

constexpr float PHI = 0.0f;
constexpr float THETA = 0.0f;

class RadialCamera : public Camera3D {
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Center;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // radial parameters
    float Phi;
    float Theta;
    float Radius;

public:
    RadialCamera(
            glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float radiusVector = 1.0f,
            float width = 4,
            float height = 3,
            float phi = PHI,
            float theta = THETA,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
    );

    glm::mat4 GetViewMatrix() const override;
    glm::mat4 GetProjectionMatrix() const override;

    /**
    * @name ProcessMouseMovement - Processes the change of perspective around Center point by mouse movement
    * @param xoffset - Value of mouse xoffset
    * @param yoffset - Value of mouse yoffset
    * @param constrainTheta - Whether to constrain angles to 90 degree max
    * @return void
    */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainTheta = true);

    /**
    * @name ProcessMouseScroll - Processes zoom and radius magnitude
    * @param yoffset - Value of movement
    * @return void
    */
    void ProcessMouseScroll(float yoffset);

private:
    void UpdateCameraVectors();
};

}

#endif // RADIALCAMERA_H_
