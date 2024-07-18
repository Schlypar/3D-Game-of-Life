#pragma once

#include <vector>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

class VertexArray {
private:
    unsigned int id;
    unsigned int vertexAttribLen;

public:
    VertexArray();
    VertexArray(const VertexArray& other);
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(VertexArray&& other);

    void SetupAttribPtr(const VertexBufferLayout& layout);

    void Bind();
    void Unbind() const;

    static std::vector<VertexArray> GenerateBuffers(unsigned int num_of_buffs);

};

}
