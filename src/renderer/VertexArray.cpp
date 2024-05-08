#include "VertexArray.h"
#include "glad/gl.h"

namespace GoL {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
}

VertexArray::VertexArray(const VertexArray& other)
    : id(other.id) {
}

VertexArray::VertexArray(VertexArray&& other)
    : id(other.id) {
    other.id = 0;
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

VertexArray& VertexArray::operator=(const VertexArray& other) {
    id = other.id;
    return *this;
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    id = other.id;
    other.id = 0;

    return *this;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int gloffset) {
    this->Bind();
    vb.Bind();

    const std::vector<VertexBufferLayout::Element>& elements = layout.GetLayoutElements();

    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const VertexBufferLayout::Element& element = elements[i];
        glEnableVertexAttribArray(gloffset + i);
        glVertexAttribPointer(gloffset + i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
        offset += element.count * VertexBufferLayout::Element::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(id);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

}
