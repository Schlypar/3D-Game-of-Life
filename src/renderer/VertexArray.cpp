#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glad/gl.h"

namespace GoL {

VertexArray::VertexArray() {
    id = 0;
    vertexAttribLen = 0;
}

VertexArray::VertexArray(const VertexArray& other) {
    id = other.id;
    vertexAttribLen = other.vertexAttribLen;
}

VertexArray::VertexArray(VertexArray&& other) {
    std::swap(id, other.id);
    std::swap(vertexAttribLen, other.vertexAttribLen);
}

VertexArray::~VertexArray() {
    Unbind();
    if (id != 0) {
        glDeleteVertexArrays(1, &id);
    }
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    std::swap(id, other.id);
    std::swap(vertexAttribLen, other.vertexAttribLen);

    return *this;
}

void VertexArray::SetupAttribPtr(const VertexBufferLayout& layout) {
    const std::vector<VertexBufferLayout::Element>& elements = layout.GetLayoutElements();

    for (int i = elements.size(); i < vertexAttribLen; i++) {
        glDisableVertexAttribArray(i);
    }

    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const VertexBufferLayout::Element& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
        offset += element.count * VertexBufferLayout::Element::GetSizeOfType(element.type);
    }

    vertexAttribLen = elements.size();
}

void VertexArray::Bind() {
    if (id == 0) {
        glGenVertexArrays(1, &id);
    }
    glBindVertexArray(id);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

std::vector<VertexArray> VertexArray::GenerateBuffers(unsigned int num_of_buffs) {
    auto vaos = std::vector<VertexArray>(num_of_buffs, VertexArray());

    GLuint* vbos_id = new GLuint[num_of_buffs];
    glGenVertexArrays(num_of_buffs, vbos_id);
    
    for (int i = 0; i < num_of_buffs; i++) {
        vaos[i].id = vbos_id[i];
    }
    delete[] vbos_id;

    return vaos;
}


}
