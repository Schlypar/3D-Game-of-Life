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

    UnindexedMesh& operator+=(const UnindexedMesh& other) {
        size_t newSize = data.size + other.data.size;
        this->data.bytes = new Byte[newSize];

        const Byte* currentBytes = this->data.bytes;
        std::memcpy(this->data.bytes, currentBytes, data.size);
        std::memcpy(this->data.bytes + data.size, other.data.bytes, other.data.size);

        this->data.size = newSize;
        this->data.usage = GL_DYNAMIC_DRAW;

        delete[] currentBytes;

        return *this;
    }

    void Bind() override {
        this->vertexBuffer.Bind();
        this->vertexBuffer.Realloc(data.size, data.usage);
        this->vertexBuffer.Write(data.bytes, data.size);
        this->vertexArray.Bind();
    }

    bool IsIndexed() override {
        return false;
    }
};

}