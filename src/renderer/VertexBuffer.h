#pragma once

#include <glad/gl.h>

#include <cstddef>
#include <vector>

namespace GoL {

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer();
    VertexBuffer(const void* data, const size_t size, GLenum usage = GL_STATIC_DRAW);
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    VertexBuffer& operator=(VertexBuffer&& other);

    void Realloc(const size_t size, GLenum usage = GL_STATIC_DRAW);
    void Write(const void* data, size_t size, unsigned int offset = 0);

    void Bind();
    void Unbind() const;

    bool operator==(const VertexBuffer other);

    inline void Delete() {
        glDeleteBuffers(1, &this->id);
    }

    static std::vector<VertexBuffer> GenerateBuffers(unsigned int num_of_buffs);
};

}
