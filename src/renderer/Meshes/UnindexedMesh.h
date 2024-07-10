#pragma once

#include "Mesh.h"
#include "VertexBufferLayout.h"
#include <memory>

namespace GoL {

template <typename T>
class UnindexedMesh : public Mesh<T> {
public:
    UnindexedMesh(const T* data, const size_t size, const VertexBufferLayout& layout, GLenum usage = GL_STATIC_DRAW)
        : Mesh<T>(data, size, layout, usage) {
        this->vertexArray.Unbind();
        this->vertexBuffer.Unbind();
    }

    ~UnindexedMesh() = default;

    std::shared_ptr<UnindexedMesh<T>> operator+(const UnindexedMesh<T>* right) {
        size_t newSize = this->data.size + right->data.size;
        T* bytes = new T[newSize];

        auto thisCount = (this->data.size / sizeof(T));
        auto otherCount = (right->data.size / sizeof(T));
        for (int i = 0; i < thisCount; i++) {
            bytes[i] = this->data.bytes[i];
        }
        for (int i = 0; i < otherCount; i++) {
            bytes[i + thisCount] = this->data.bytes[i];
        }

        return std::make_shared<UnindexedMesh<T>>(bytes, newSize, this->GetLayout(), GL_DYNAMIC_DRAW);
    }

    UnindexedMesh<T>& operator+=(const UnindexedMesh<T>& other) {
        const T* currentTs = this->data.bytes;

        size_t newSize = this->data.size + other.data.size;
        this->data.bytes = new T[newSize];

        std::memcpy(this->data.bytes, currentTs, (this->data.size / sizeof(T)));
        std::memcpy(this->data.bytes + (this->data.size / sizeof(T)), other.data.bytes, other.data.size);

        this->data.size = newSize;
        this->data.usage = GL_DYNAMIC_DRAW;

        delete[] currentTs;

        return *this;
    }

    void Bind() override {
        this->vertexArray.Bind();
        this->vertexBuffer.Bind();
    }

    void Unbind() override {
        this->vertexArray.Unbind();
        this->vertexBuffer.Unbind();
    }

    void Resize() override {
        this->vertexBuffer.Realloc(this->data.size, this->data.usage);
        this->vertexBuffer.Write(this->data.bytes, this->data.size);
    }

    bool IsIndexed() override {
        return false;
    }

    Mesh<T>::Data& GetData() override {
        return this->data;
    }
};

}
