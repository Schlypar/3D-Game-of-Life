#pragma once

#include <glad/gl.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <range/v3/algorithm/for_each.hpp>

#include "Surface.h"

namespace GoL {

template <typename T>
class Model {
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    float scaleFactor;

    bool changed;
    glm::mat4 modelMatrix;

    MaterialPointer materialPointer;

protected:
    Model(const glm::vec3& position, const glm::vec3& rotation, float scaleFactor)
        : position(position)
        , rotation(rotation)
        , scaleFactor(scaleFactor)
        , changed(true) {
    }

public:
    virtual ~Model() = default;

    void SetPosition(const glm::vec3& position) {
        this->position = position;
        changed = true;
    }

    void SetRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
        changed = true;
    }

    void SetScaleFactor(float scaleFactor) {
        this->scaleFactor = scaleFactor;
        changed = true;
    }

    glm::mat4& GetModelMatrix() {
        if (changed) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, position);
            modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
            modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
        }

        changed = false;
        return modelMatrix;
    }

    MaterialPointer& GetMaterial() {
        return this->materialPointer;
    }

    virtual void SetMaterial(const MaterialPointer& material) = 0;

    virtual std::vector<Surface<T>> GetSurfaces() const = 0;
};
}
