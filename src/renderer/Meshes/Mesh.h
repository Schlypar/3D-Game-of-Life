#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <cstring>

namespace GoL {

template <typename T>
class Mesh {
public:
    struct Data {
        T* bytes;
        size_t size;
        GLenum usage;

        Data(const void* data, const size_t size, GLenum usage = GL_STATIC_DRAW)
            : bytes(new T[size])
            , size(size) {
            std::memcpy(this->bytes, data, size);
        }

        Data(const Data& other)
            : bytes(new T[other.size])
            , size(other.size)
            , usage(other.usage) {
            std::memcpy(this->bytes, bytes, size);
        }

        Data(Data&& other)
            : bytes(other.bytes)
            , size(other.size)
            , usage(other.usage) {
            other.bytes = nullptr;
            other.size = 0;
            other.usage = 0;
        }

        ~Data() {
            if (bytes != nullptr) {
                delete[] bytes;
            }
        }

        Data& operator=(const Data& other) {
            bytes = other.bytes;
            size = other.size;
            usage = other.usage;

            return *this;
        }

        Data& operator=(Data&& other) {
            if (this == &other) {
                return *this;
            }
            bytes = other.bytes;
            size = other.size;
            usage = other.usage;

            other.bytes = nullptr;
            other.size = 0;
            other.usage = 0;

            return *this;
        }
    };

protected:
    Data data;
    VertexBuffer vertexBuffer;
    VertexArray vertexArray;

public:
    Mesh(const T* data, const size_t size, GLenum usage = GL_STATIC_DRAW)
        : data(data, size, usage)
        , vertexBuffer(data, size, usage)
        , vertexArray() {
    }

    Mesh(const T* data, const size_t size, const VertexBufferLayout& layout, GLenum usage = GL_STATIC_DRAW)
        : data(data, size, usage)
        , vertexBuffer(data, size, usage)
        , vertexArray() {
        vertexArray.AddBuffer(vertexBuffer, layout);
    }

    void AddLayout(const VertexBufferLayout& layout) {
        vertexArray.AddBuffer(vertexBuffer, layout);
    }

    virtual ~Mesh() = default;

    virtual void Bind() = 0;
    virtual void Resize() = 0;
    virtual bool IsIndexed() = 0;
    virtual Data& GetData() = 0;
};

}
