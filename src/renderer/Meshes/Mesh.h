#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

class Mesh {
protected:
    VertexBuffer vertexBuffer;
    VertexArray vertexArray;

public:
    Mesh(const void* data, const size_t size)
        : vertexBuffer(data, size)
        , vertexArray() {
    }

    Mesh(const void* data, const size_t size, const VertexBufferLayout& layout)
        : vertexBuffer(data, size)
        , vertexArray() {
        vertexArray.AddBuffer(vertexBuffer, layout);
    }

    void AddLayout(const VertexBufferLayout& layout) {
        vertexArray.AddBuffer(vertexBuffer, layout);
    }

    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual bool IsIndexed() const = 0;
};

}