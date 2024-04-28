#pragma once

#include <cstddef>

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer(const void* data, const size_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};
