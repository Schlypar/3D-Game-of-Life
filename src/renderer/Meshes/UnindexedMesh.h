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

    UnindexedMesh(UnindexedMesh& oth) {
        this->vertexArray = oth.vertexArray;
        this->vertexBuffer = oth.vertexBuffer;
        this->layout = oth.layout;
        this->data = oth.data;
    }

    ~UnindexedMesh() = default;

    std::shared_ptr<UnindexedMesh<T>> operator+(const UnindexedMesh<T>* right) {
        size_t newSize = this->data.size + right->data.size;
        T* bytes = new T[newSize / sizeof(T)];

        std::memcpy(bytes, this->data.bytes, this->data.size);
        std::memcpy((char*)bytes + this->data.size, right->data.bytes, right->data.size);

        auto res = std::make_shared<UnindexedMesh<T>>(bytes, newSize, this->GetLayout(), GL_DYNAMIC_DRAW);
        delete[] bytes;
        return res;
    }

    UnindexedMesh<T>& operator+=(const UnindexedMesh<T>& other) {
        const T* oldData = this->data.bytes;

        size_t newSize = this->data.size + other.data.size;
        this->data.bytes = new T[newSize / sizeof(T)];

        std::memcpy(bytes, this->data.bytes, this->data.size);
        std::memcpy((char*)bytes + this->data.size, right->data.bytes, right->data.size);

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
};

}
