#pragma once

#include <algorithm>
#include <iostream>
#include <memory>

#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Meshes/UnindexedMesh.h"
#include "Vertex.h"

#include "Materials/Material.h"
#include "Meshes/Mesh.h"

namespace GoL {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
struct Surface {
    GLenum mode;
    unsigned int vertexCount;
    Ref<Mesh<T>> mesh;
    Ref<Material> material;

    Surface()
        : mode(GL_TRIANGLES)
        , vertexCount(0)
        , mesh(nullptr)
        , material(nullptr) {
    }

    Surface(GLenum mode, unsigned int vertexCount, Ref<Mesh<T>> mesh, Ref<Material> material)
        : mode(mode)
        , vertexCount(vertexCount)
        , mesh(mesh)
        , material(material) {
    }

    Surface& operator+=(const Surface& other) {
        if (this->mesh->IsIndexed()) {
            // DEPRECATED
            return *this;
        } else {
            auto thisMesh = static_cast<UnindexedMesh<T>*>(this->mesh.get());
            auto otherMesh = static_cast<UnindexedMesh<T>*>(other.mesh.get());

            *thisMesh += *otherMesh;

            return *this;
        }
    }

    friend Surface operator+(const Surface& left, const Surface& right) {
        if (left.mesh->IsIndexed()) {
            // DEPRECATED
            return left;
        } else {
            auto leftMesh = static_cast<UnindexedMesh<T>*>(left.mesh.get());
            auto rightMesh = static_cast<UnindexedMesh<T>*>(right.mesh.get());

            return Surface { left.mode, left.vertexCount + right.vertexCount, *leftMesh + rightMesh, left.material };
        }
    }
};

template <typename T>
class SurfaceBuilder {
private:
    Ref<Mesh<T>> mesh;
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

    void SetMesh(Ref<Mesh<T>> mesh) {
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

    Surface<T> Build() const {
        if (mesh == nullptr || material == nullptr || vertexCount == 0) {
            std::cout << "Error: mesh, material or vertex count not set" << std::endl;
            return Surface<T> { mode, 0, nullptr, nullptr };
        }
        return Surface<T> { mode, vertexCount, mesh, material };
    }
};

template <typename T>
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

    virtual std::vector<Surface<T>> GetSurfaces() const = 0;
};
}
