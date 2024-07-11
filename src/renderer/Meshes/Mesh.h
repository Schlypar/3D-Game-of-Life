#pragma once

#include <cstring>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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
    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    VertexBufferLayout layout;

public:
    Mesh(const T* data, const size_t size, const VertexBufferLayout& layout, GLenum usage = GL_STATIC_DRAW)
        : data(data, size, usage)
        , vertexArray()
        , vertexBuffer(nullptr, 1, usage) {
        this->vertexArray = VertexArray();
        this->vertexArray.Bind();

        this->vertexBuffer = VertexBuffer(data, size, usage);

        vertexArray.AddBuffer(vertexBuffer, layout);
        this->vertexBuffer.Unbind();

        this->layout = layout;
    }

    virtual ~Mesh() = default;

    void AddLayout(const VertexBufferLayout& layout) {
        vertexArray.AddBuffer(vertexBuffer, layout);
    }

    const VertexBufferLayout& GetLayout() const {
        return this->layout;
    }

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Resize() = 0;
    virtual bool IsIndexed() = 0;
    virtual Data& GetData() = 0;
};

}
