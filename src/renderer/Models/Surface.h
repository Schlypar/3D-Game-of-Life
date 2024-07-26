#ifndef SURFACE_H_
#define SURFACE_H_

#include "precompiled.h"

#include "range/v3/algorithm/for_each.hpp"

#include "MaterialLibrary.h"
#include "Materials/Material.h"

#include "Vertex.h"

#include "Meshes/UnindexedMesh.h"

namespace GoL {

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
struct Surface {
    GLenum mode;
    unsigned int vertexCount;
    Mesh<T>* mesh;
    Material* material;

    Surface()
        : mode(GL_TRIANGLES)
        , vertexCount(0)
        , mesh(nullptr)
        , material(nullptr) {
    }

    Surface(GLenum mode, unsigned int vertexCount, Mesh<T>* mesh, Material* material)
        : mode(mode)
        , vertexCount(vertexCount)
        , mesh(mesh)
        , material(material) {
    }

    Surface(const Surface<T>& other) {
        this->mode = other.mode;
        this->vertexCount = other.vertexCount;
        this->material = other.material;

        auto& otherData = other.mesh->GetData();
        this->mesh = new UnindexedMesh<T>(otherData.bytes, other.vertexCount * sizeof(T), other.mesh->GetLayout(), GL_STATIC_DRAW);
    }

    Surface(Surface<T>&& other)
        : mode(other.mode)
        , vertexCount(other.vertexCount)
        , mesh(other.mesh)
        , material(other.material) {
        other.mode = 0;
        other.vertexCount = 0;
        other.mesh = nullptr;
        other.material = nullptr;
    }

    ~Surface() {
        mode = 0;
        vertexCount = 0;

        if (mesh != nullptr) {
            delete mesh;
            mesh = nullptr;
        }
    }

    Surface<T>& operator=(const Surface<T>& other) {
        if (this == &other) {
            return *this;
        }
        this->mode = other.mode;
        this->vertexCount = other.vertexCount;
        this->material = other.material;

        auto& otherData = other.mesh->GetData();
        delete this->mesh;
        this->mesh = new UnindexedMesh<T>(otherData.bytes, other.vertexCount * sizeof(T), other.mesh->GetLayout(), GL_STATIC_DRAW);
        // this->mesh->Resize();

        return *this;
    }

    Surface<T>& operator=(Surface<T>&& other) {
        if (this == &other) {
            return *this;
        }
        this->mode = other.mode;
        this->vertexCount = other.vertexCount;
        this->material = other.material;
        delete this->mesh;
        this->mesh = other.mesh;

        other.mode = 0;
        other.vertexCount = 0;
        other.mesh = nullptr;
        other.material = nullptr;

        return *this;
    }

    Surface& operator+=(const Surface& other) {
        if (this->mesh->IsIndexed()) {
            // DEPRECATED
            return *this;
        } else {
            auto thisMesh = static_cast<UnindexedMesh<T>*>(this->mesh);
            auto otherMesh = static_cast<UnindexedMesh<T>*>(other.mesh);

            *thisMesh += *otherMesh;
            this->vertexCount += other.vertexCount;

            return *this;
        }
    }

    friend Surface operator+(const Surface& left, const Surface& right) {
        if (left.mesh->IsIndexed()) {
            // DEPRECATED
            return left;
        } else {
            auto leftMesh = static_cast<UnindexedMesh<T>*>(left.mesh);
            auto rightMesh = static_cast<UnindexedMesh<T>*>(right.mesh);

            return Surface { left.mode, left.vertexCount + right.vertexCount, *leftMesh + rightMesh, left.material };
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Surface<T>& s) {
        os << "Surface {\n\tMode: " << s.mode
           << "\n\tVertex Count: " << s.vertexCount
           << "\n\tVertices: {\n";
        ranges::for_each(
                s.mesh->GetData().bytes,
                s.mesh->GetData().bytes + s.vertexCount,
                [&os](const Vertex& x) { os << "\t\t" << x << "\n"; }
        );
        os << "\t}\n}\n";
        return os;
    }
};

template <typename T>
class SurfaceBuilder {
private:
    Mesh<T>* mesh;
    Material* material;
    unsigned int vertexCount;
    unsigned int mode = GL_TRIANGLES;

public:
    SurfaceBuilder()
        : mesh(nullptr)
        , material(nullptr)
        , vertexCount(0) {
    }

    ~SurfaceBuilder() = default;

    void SetMesh(Mesh<T>* mesh) {
        this->mesh = mesh;
    }

    void SetMaterial(Material* material) {
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
        return Surface<T>(mode, vertexCount, mesh, material);
    }
};

}


#endif // SURFACE_H_
