#include "VertexBuffer.h"

#include "Renderer.h"

namespace GoL {

VertexBuffer::VertexBuffer(const void* data, const size_t size, GLenum usage) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    OPENGL_INFO("Created VBO with id {} of size {} and usage {}", id, size, usage);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) {
    std::swap(id, other.id);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    std::swap(id, other.id);

    return *this;
}

void VertexBuffer::Realloc(const size_t size, GLenum usage) {
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
}

void VertexBuffer::Write(const void* data, size_t size, unsigned int offset) {
    glNamedBufferSubData(id, offset, size, data);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
