#pragma once

#include <glad/gl.h>

#include <cstddef>

namespace GoL {

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer(const void* data, const size_t size, GLenum usage = GL_STATIC_DRAW);
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    VertexBuffer& operator=(VertexBuffer&& other);

    void Realloc(const size_t size, GLenum usage = GL_STATIC_DRAW);
    void Write(const void* data, size_t size, unsigned int offset = 0);

    void Bind() const;
    void Unbind() const;

    inline void Delete() {
        glDeleteBuffers(1, &this->id);
    }
};

}
