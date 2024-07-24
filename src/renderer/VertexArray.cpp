#include <glad/gl.h>

#include "precompiled.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
    OPENGL_INFO("VAO with id {} has been created", id);
}

VertexArray::VertexArray(VertexArray&& other) {
    std::swap(id, other.id);
}

VertexArray::~VertexArray() {
    Unbind();
    OPENGL_INFO("VAO with id {} has been destroyed", id);
    glDeleteVertexArrays(1, &id);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    std::swap(id, other.id);

    return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    this->Bind();
    vb.Bind();

    const std::vector<VertexBufferLayout::Element>& elements = layout.GetLayoutElements();

    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const VertexBufferLayout::Element& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
        offset += element.count * VertexBufferLayout::Element::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    OPENGL_INFO("VAO with id {} was binded", id);
    glBindVertexArray(id);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

void VertexArray::Unbind() const {
    OPENGL_INFO("VAO with id {} was unbinded", id);
    glBindVertexArray(0);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        auto errorString = glad_glGetError();
        OPENGL_ERROR("Error {}: {}", errorCode, errorString);
    }
}

}
