#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Shader& shader) const;
};