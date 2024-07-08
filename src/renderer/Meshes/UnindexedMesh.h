#pragma once

#include "Mesh.h"
#include "VertexBufferLayout.h"
#include <memory>

namespace GoL {

class UnindexedMesh : public Mesh {
public:
    UnindexedMesh(const void* data, const size_t size, GLenum usage = GL_STATIC_DRAW)
        : Mesh(data, size, usage) {
    }

    UnindexedMesh(const void* data, const size_t size, const VertexBufferLayout& layout, GLenum usage = GL_STATIC_DRAW)
        : Mesh(data, size, layout, usage) {
    }

    ~UnindexedMesh() = default;

    std::shared_ptr<UnindexedMesh> operator+(const UnindexedMesh* right) {
        // presume that it is always the case
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        size_t newSize = this->data.size + right->data.size;
        Byte* bytes = new Byte[newSize];

        std::memcpy(bytes, this->data.bytes, this->data.size);
        std::memcpy(bytes + this->data.size, right->data.bytes, right->data.size);

        return std::make_shared<UnindexedMesh>((const void*) bytes, newSize, layout, GL_DYNAMIC_DRAW);
    }

    UnindexedMesh& operator+=(const UnindexedMesh& other) {
        const Byte* currentBytes = this->data.bytes;

        size_t newSize = data.size + other.data.size;
        this->data.bytes = new Byte[newSize];

        std::memcpy(this->data.bytes, currentBytes, data.size);
        std::memcpy(this->data.bytes + data.size, other.data.bytes, other.data.size);

        this->data.size = newSize;
        this->data.usage = GL_DYNAMIC_DRAW;

        delete[] currentBytes;

        return *this;
    }

    void Bind() override {
        this->vertexBuffer.Bind();
        this->vertexArray.Bind();
    }

    void Resize() override {
        this->vertexBuffer.Realloc(data.size, data.usage);
        this->vertexBuffer.Write(data.bytes, data.size);
    }

    bool IsIndexed() override {
        return false;
    }

    Mesh::Data& GetData() override {
        return this->data;
    }
};

}
