#pragma once

namespace GoL {

class IndexBuffer {
private:
    unsigned int id;
    unsigned int size;

public:
    IndexBuffer(const unsigned int* data, unsigned int size);
    IndexBuffer(const IndexBuffer& other);
    IndexBuffer(IndexBuffer&& other);
    ~IndexBuffer();

    IndexBuffer& operator=(const IndexBuffer& other);
    IndexBuffer& operator=(IndexBuffer&& other);

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const {
        return size;
    }
};

}
