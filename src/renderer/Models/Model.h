#pragma once

#include <memory>

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Materials/Material.h"
#include "Meshes/Mesh.h"

namespace GoL {

template <typename T>
using Ref = std::shared_ptr<T>;

struct Surface {
    unsigned int vertexCount;
    Ref<Mesh> mesh;
    Ref<Material> material;
};

class Model {
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    float scaleFactor;

protected:
    Model(const glm::vec3& position, const glm::vec3& rotation, float scaleFactor)
        : position(position)
        , rotation(rotation)
        , scaleFactor(scaleFactor) {
    }

public:
    virtual ~Model() = default;

    void SetPosition(const glm::vec3& position) {
        this->position = position;
    }

    void SetRotation(const glm::vec3& rotation) {
        this->rotation = rotation;
    }

    void SetScaleFactor(float scaleFactor) {
        this->scaleFactor = scaleFactor;
    }

    glm::mat4 GetModelMatrix() const {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));

        return modelMatrix;
    }

    virtual std::vector<Surface> GetSurfaces() const = 0;
};
}