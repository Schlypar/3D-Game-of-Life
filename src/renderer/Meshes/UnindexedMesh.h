#pragma once

#include "Mesh.h"
#include "VertexBufferLayout.h"

namespace GoL {

class UnindexedMesh : public Mesh {

public:
    UnindexedMesh(const void* data, const size_t size)
        : Mesh(data, size) {
    }

    UnindexedMesh(const void* data, const size_t size, const VertexBufferLayout& layout)
        : Mesh(data, size, layout) {
    }

    ~UnindexedMesh() = default;

    void Bind() const override {
        this->vertexBuffer.Bind();
        this->vertexArray.Bind();
    }

    bool IsIndexed() const override {
        return false;
    }
};

}