#pragma once

#include "glad/gl.h"
#include <cassert>
#include <vector>

namespace Gol {

class VertexBufferLayout {
public:
    struct Element {
        unsigned int type;
        unsigned int count;
        unsigned int normalized;

        static unsigned int GetSizeOfType(unsigned int type) {
            switch (type) {
                case GL_FLOAT:
                    return 4;
                case GL_UNSIGNED_INT:
                    return 4;
                case GL_UNSIGNED_BYTE:
                    return 1;
            }

            assert(false);
            return 0;
        }
    };

private:
    std::vector<Element> elements;
    unsigned int stride;

public:
    VertexBufferLayout()
        : elements()
        , stride(0) {
    }

    ~VertexBufferLayout() = default;

    template <typename T>
    void Push(unsigned int count) {
        assert(false);
    }

    template <>
    void Push<float>(unsigned int count) {
        elements.push_back({ GL_FLOAT, count, GL_FALSE });
        stride += count * Element::GetSizeOfType(GL_FLOAT);
    }

    template <>
    void Push<unsigned int>(unsigned int count) {
        elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride += count * Element::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template <>
    void Push<unsigned char>(unsigned int count) {
        elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride += count * Element::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<Element>& GetLayoutElements() const {
        return this->elements;
    }

    inline unsigned int GetStride() const {
        return this->stride;
    }
};

}
