#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace GoL {

class Renderer {
public:
    void Clear() const;
    void Draw(const GoL::VertexBuffer& vertexBuffer, const GoL::IndexBuffer& indexBuffer, const GoL::Shader& shader) const;
};

}
