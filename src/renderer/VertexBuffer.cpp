#include "VertexBuffer.h"

#include "Renderer.h"

namespace GoL {

VertexBuffer::VertexBuffer(const void* data, const size_t size, GLenum usage) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    : id(other.id) {
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
    : id(other.id) {
    other.id = 0;
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {
    this->id = other.id;
    return *this;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    this->id = other.id;
    other.id = 0;

    return *this;
}

void VertexBuffer::Realloc(const size_t size, GLenum usage) {
    glDeleteBuffers(1, &id);
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
}

void VertexBuffer::Write(const void* data, size_t size, unsigned int offset) {
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
