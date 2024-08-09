#pragma once

#include <glad/gl.h>

#include <cstddef>

namespace GoL {

class VertexBuffer {
private:
    unsigned int id;

public:
    VertexBuffer(const void* data, const size_t size, GLenum usage = GL_STATIC_DRAW);
    VertexBuffer(const VertexBuffer& other);
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    VertexBuffer& operator=(VertexBuffer&& other);

    /**
    * @name Realloc - Reallocates to size gl buffer object
    * @param size -  Size in bytes of gl buffer
    * @param usage -  Usage of buffer (see GLenum)
    * @return void
    */
    void Realloc(const size_t size, GLenum usage = GL_STATIC_DRAW);

    /**
    * @name Write - Writes data to gl buffer object
    * @param data - Data to be written in buffer
    * @param size -  Size in bytes of data
    * @param offset - Offset in bytes from pointer
    * @return void
    */
    void Write(const void* data, size_t size, unsigned int offset = 0);

    /**
    * @name Bind - Tells OpenGL to use this GL_ARRAY_BUFFER
    * @return void
    */
    void Bind() const;

    /**
    * @name Unbind - Tells OpenGL to not use any GL_ARRAY_BUFFER
    * @return void
    */
    void Unbind() const;

    inline unsigned int GetID() {
        return id;
    }
};

}
