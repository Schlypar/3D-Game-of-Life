#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

class VertexArray {
private:
    unsigned int id;

public:
    VertexArray();
    VertexArray(const VertexArray& other);
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(const VertexArray& other);
    VertexArray& operator=(VertexArray&& other);

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int offset = 0);

    void Bind() const;
    void Unbind() const;
};

}
