#pragma once

#include <iostream>
#include <memory>

#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Vertex.h"

#include "Materials/Material.h"
#include "Meshes/Mesh.h"

namespace GoL {

template <typename T>
using Ref = std::shared_ptr<T>;

struct Surface {
    GLenum mode;
    unsigned int vertexCount;
    Ref<Mesh> mesh;
    Ref<Material> material;
};

class SurfaceBuilder {
private:
    Ref<Mesh> mesh;
    Ref<Material> material;
    unsigned int vertexCount;
    unsigned int mode = GL_TRIANGLES;

public:
    SurfaceBuilder()
        : mesh(nullptr)
        , material(nullptr)
        , vertexCount(0) {
    }

    ~SurfaceBuilder() = default;

    void SetMesh(Ref<Mesh> mesh) {
        this->mesh = mesh;
    }

    void SetMaterial(Ref<Material> material) {
        this->material = material;
    }

    void SetVertexCount(unsigned int vertexCount) {
        this->vertexCount = vertexCount;
    }

    void SetMode(GLenum mode) {
        this->mode = mode;
    }

    Surface Build() const {
        if (mesh == nullptr || material == nullptr || vertexCount == 0) {
            std::cout << "Error: mesh, material or vertex count not set" << std::endl;
            return Surface { mode, 0, nullptr, nullptr };
        }
        return Surface { mode, vertexCount, mesh, material };
    }
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
