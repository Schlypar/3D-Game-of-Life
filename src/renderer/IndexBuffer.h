#pragma once

namespace GoL {

class IndexBuffer {
private:
    unsigned int id;
    unsigned int count;

public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    IndexBuffer(const IndexBuffer& other);
    IndexBuffer(IndexBuffer&& other);
    ~IndexBuffer();

    IndexBuffer& operator=(const IndexBuffer& other);
    IndexBuffer& operator=(IndexBuffer&& other);

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const {
        return count;
    }
};

}
