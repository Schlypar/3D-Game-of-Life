#ifndef RADIALCAMERA_H_
#define RADIALCAMERA_H_

#include "precompiled.h"

#include <glad/gl.h>

namespace GoL {

// defaults
constexpr float PHI2 = 0.0f;
constexpr float THETA2 = 0.0f;
constexpr float SPEED2 = 2.5f;
constexpr float SENSITIVITY2 = 1.0f;
constexpr float ZOOM2 = 45.0f;

class RadialCamera {
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

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float FoV;

    // perspective parameters
    float width;
    float height;
    float nearPlane;
    float farPlane;

public:
    RadialCamera(
            glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float radiusVector = 1.0f,
            float width = 4,
            float height = 3,
            float phi = PHI2,
            float theta = THETA2,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
    );

    /**
    * @name GetViewMatrix - Returns matrix that transorms coordinates into the coordinates of observer
    * @return glm::mat4 - View matrix in MVP model
    */
    glm::mat4 GetViewMatrix() const;

    /**
    * @name GetProjectionMatrix - Returns matrix that transorms coordinates into normal coordinates
    * @return glm::mat4 - Projection matrix in MVP model
    */
    glm::mat4 GetProjectionMatrix() const;

    /**
    * @name ProcessMouseMovement - Processes the change of perspective by mouse movement
    * @param xoffset - Value of mouse xoffset
    * @param yoffset - Value of mouse yoffset
    * @param constrainPitch - Whether to constrain angles to 90 degree max
    * @return void
    */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainTheta = true);

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

#endif // RADIALCAMERA_H_
