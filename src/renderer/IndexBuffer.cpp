#include "IndexBuffer.h"
#include "glad/gl.h"
#include <cstring>
#include <stdio.h>

namespace GoL {

// init
unsigned int* IndexBuffer::all_indices = nullptr;
int IndexBuffer::size = 0;
unsigned int IndexBuffer::id = 0;
std::vector<int> IndexBuffer::index_offsets = std::vector<int>();

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &IndexBuffer::id);
    delete[] all_indices;
}

IndexBuffer::Id IndexBuffer::Register(const unsigned int* indices, int _size) {
    unsigned int* ptr = new unsigned int[size + _size];
    std::memcpy(ptr, all_indices, size * sizeof(unsigned int));
    std::memcpy(ptr + size, indices, _size * sizeof(unsigned int));
    index_offsets.push_back(size);
    size += _size;
    delete[] all_indices;
    all_indices = ptr;
    return index_offsets.size() - 1;
}

void IndexBuffer::Init() {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), all_indices, GL_STATIC_DRAW);   
}

void* IndexBuffer::GetOffset(Id id) {
    return (void*)(all_indices + index_offsets[id]);
}

}
