#pragma once

#include "IndexBuffer.h"
#include "Mesh.h"
#include "VertexBufferLayout.h"

namespace GoL {

class IndexedMesh : public Mesh {
private:
    IndexBuffer::Id* indexBuffer;

public:
    IndexedMesh(const void* data, const size_t size, const unsigned int* indices, int count)
        : Mesh(data, size)
        , indexBuffer(new IndexBuffer::Id) {
        *indexBuffer = IndexBuffer::Register(indices, count);
    }

    IndexedMesh(const void* data, const size_t size, const unsigned int* indices, int count, const VertexBufferLayout& layout)
        : Mesh(data, size, layout)
        , indexBuffer(new IndexBuffer::Id) {
        *indexBuffer = IndexBuffer::Register(indices, count);
    }

    ~IndexedMesh() {
        if (indexBuffer != nullptr) {
            delete indexBuffer;
        }
    }

    IndexedMesh& operator+=(const IndexedMesh& other) {
        // TODO

        return *this;
    }

    const IndexBuffer::Id* GetIndexBuffer() const {
        return indexBuffer;
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
        return true;
    }
};

}
