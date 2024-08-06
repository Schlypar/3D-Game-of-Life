#ifndef CAMERA3D_H_
#define CAMERA3D_H_

#include "precompiled.h"

#include <glad/gl.h>

namespace GoL {

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// defaults
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera3D {
public:
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
    Camera3D(
            float movementSpeed = SPEED,
            float mouseSensitivity = SENSITIVITY,
            float FoV = ZOOM,
            float width = 4,
            float height = 3,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
    )
        : MovementSpeed(movementSpeed)
        , MouseSensitivity(mouseSensitivity)
        , FoV(FoV)
        , width(width)
        , height(height)
        , nearPlane(nearPlane)
        , farPlane(farPlane) {
    }

    virtual ~Camera3D() = default;

    /**
    * @name GetViewMatrix - Returns matrix that transorms coordinates into the coordinates of observer
    * @return glm::mat4 - View matrix in MVP model
    */
    virtual glm::mat4 GetViewMatrix() const = 0;

    /**
    * @name GetProjectionMatrix - Returns matrix that transorms coordinates into normal coordinates
    * @return glm::mat4 - Projection matrix in MVP model
    */
    virtual glm::mat4 GetProjectionMatrix() const = 0;
};

}

#endif // CAMERA3D_H_
