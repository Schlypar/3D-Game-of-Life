#include "VertexBuffer.h"

#include "Renderer.h"

namespace GoL {

VertexBuffer::VertexBuffer(const void* data, const size_t size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
