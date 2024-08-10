#include "VertexBuffer.h"

#include "precompiled.h"

#include "glad/gl.h"

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
    OPENGL_INFO("VAO with id {} has been destroyed", id);
    glDeleteBuffers(1, &id);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) {
    std::swap(id, other.id);

    return *this;
}

void VertexBuffer::Realloc(const size_t size, GLenum usage) {
    OPENGL_INFO("VAO with id {} was reallocated to size {} bytes and usage {}", id, size, usage);
    glNamedBufferData(id, size, nullptr, usage);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

void VertexBuffer::Write(const void* data, size_t size, unsigned int offset) {
    glNamedBufferSubData(id, offset, size, data);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    } else {
        OPENGL_INFO("Data of size {} has been written in VAO with id {} at offest {} bytes", size, id, offset);
    }
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    } else {
        OPENGL_INFO("VAO with id {} was binded", id);
    }
}

void VertexBuffer::Unbind() const {
    OPENGL_INFO("VAO with id {} was unbinded", id);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

}
