#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"

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

    virtual ~Mesh() = default;

    virtual void Bind() const = 0;
    virtual bool IsIndexed() const = 0;
};

}