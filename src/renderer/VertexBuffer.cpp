#include "VertexBuffer.h"

#include "Renderer.h"

namespace GoL {

VertexBuffer::VertexBuffer() {
    id = 0;
}

VertexBuffer::VertexBuffer(const void* data, const size_t size, GLenum usage) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::VertexBuffer(const VertexBuffer& other)
    : id(other.id) {
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
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() {
    if (id == 0) {
        glGenBuffers(1, &id);
    }
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool VertexBuffer::operator==(const VertexBuffer other) {
    return id == other.id;
}

std::vector<VertexBuffer> VertexBuffer::GenerateBuffers(unsigned int num_of_buffs) {
    auto vbos = std::vector<VertexBuffer>(num_of_buffs, VertexBuffer());

    GLuint* vbos_id = new GLuint[num_of_buffs];
    glGenBuffers(num_of_buffs, vbos_id);
    
    for (int i = 0; i < num_of_buffs; i++) {
        vbos[i].id = vbos_id[i];
    }
    delete[] vbos_id;

    return vbos;
}


}
