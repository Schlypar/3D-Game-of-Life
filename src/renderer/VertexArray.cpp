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

void VertexArray::AddBuffer(const std::vector<VertexBuffer>& vb, const VertexBufferLayout& layout) {
    this->Bind();
    vb[0].Bind();

    const std::vector<VertexBufferLayout::Element>& elements = layout.GetLayoutElements();

    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const VertexBufferLayout::Element& element = elements[i];
        if (i <= 1) {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
        } else {
            vb[1].Bind();
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const void*) ((i - 2) * sizeof(glm::vec4)));
            glVertexAttribDivisorARB(i, 1);
        }
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
