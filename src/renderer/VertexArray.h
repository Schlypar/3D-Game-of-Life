#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

class VertexArray {
private:
    unsigned int id;

public:
    VertexArray();
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(VertexArray&& other);

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

}
