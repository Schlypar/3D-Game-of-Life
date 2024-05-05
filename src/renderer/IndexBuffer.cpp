#include "IndexBuffer.h"
#include "glad/gl.h"
#include <cstring>
#include <stdio.h>

namespace GoL {

// init
unsigned int* IndexBuffer::all_indices = nullptr;
int IndexBuffer::count = 0;
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

IndexBuffer::Id IndexBuffer::Register(const unsigned int* indices, int _count) {
    unsigned int* ptr = new unsigned int[count + _count];
    std::memcpy(ptr, all_indices, count * sizeof(unsigned int));
    std::memcpy(ptr + count, indices, _count * sizeof(unsigned int));
    index_offsets.push_back(count);
    count += _count;
    delete[] all_indices;
    all_indices = ptr;
    return index_offsets.size() - 1;
}

void IndexBuffer::Init() {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), all_indices, GL_STATIC_DRAW);   
}

void* IndexBuffer::GetOffset(Id id) {
    return (void*)(all_indices + index_offsets[id]);
}

}
