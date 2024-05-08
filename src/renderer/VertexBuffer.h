#pragma once

#include <cstddef>

namespace GoL {

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer(const void* data = nullptr, const size_t size = 0);
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    VertexBuffer& operator=(const VertexBuffer& other);
    VertexBuffer& operator=(VertexBuffer&& other);

    void Bind() const;
    void Unbind() const;
};

}
