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

    UnindexedMesh<T>* operator+(const UnindexedMesh<T>* right) {
        size_t newSize = this->data.size + right->data.size;
        T* bytes = new T[newSize];

        auto thisCount = (this->data.size / sizeof(T));
        auto otherCount = (right->data.size / sizeof(T));
        for (int i = 0; i < thisCount; i++) {
            bytes[i] = this->data.bytes[i];
        }
        for (int i = 0; i < otherCount; i++) {
            bytes[i + thisCount] = right->data.bytes[i];
        }

        auto result = new UnindexedMesh<T>(bytes, newSize, this->GetLayout(), GL_DYNAMIC_DRAW);
        delete[] bytes;
        return result;
    }

    UnindexedMesh<T>& operator+=(const UnindexedMesh<T>& other) {
        const T* oldData = this->data.bytes;

        size_t newSize = this->data.size + other.data.size;
        this->data.bytes = new T[newSize];

        auto thisCount = (this->data.size / sizeof(T));
        auto otherCount = (other.data.size / sizeof(T));
        for (int i = 0; i < thisCount; i++) {
            this->data.bytes[i] = oldData[i];
        }
        for (int i = 0; i < otherCount; i++) {
            this->data.bytes[i + thisCount] = other.data.bytes[i];
        }

        this->data.size = newSize;
        this->data.usage = GL_DYNAMIC_DRAW;

        delete[] oldData;

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
