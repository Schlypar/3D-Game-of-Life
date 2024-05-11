#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <vector>

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

    void AddBuffer(const std::vector<VertexBuffer>& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};

}
