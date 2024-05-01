#include "IndexBuffer.h"
#include "glad/gl.h"

namespace GoL {

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : count(count) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const IndexBuffer& other)
    : id(other.id)
    , count(other.count) {
}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
    : id(other.id)
    , count(other.count) {
    other.id = 0;
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id);
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& other) {
    this->id = other.id;
    this->count = other.count;
    return *this;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) {
    this->id = other.id;
    this->count = other.count;

    other.id = 0;

    return *this;
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}
