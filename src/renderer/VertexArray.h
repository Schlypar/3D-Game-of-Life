#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GoL {

class VertexArray {
private:
    unsigned int id;

public:
    VertexArray();
    VertexArray(VertexArray&& other);
    ~VertexArray();

    VertexArray& operator=(VertexArray&& other);

    /**
    * @name AddBuffer - Links VAO with VBO with buffer layout
    * @param vb -  GL_ARRAY_BUFFER to be linked
    * @param layout -  Layout of the VBO
    * @return void
    */
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    /**
    * @name Bind - Tells OpenGL to use this vertex array
    * @return void
    */
    void Bind() const;

    /**
    * @name Unbind - Tells OpenGL to not use any vertex array
    * @return void
    */
    void Unbind() const;
};

}
