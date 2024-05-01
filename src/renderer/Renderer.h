#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <memory>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace GoL {

class Renderer {
public:
    void Clear() const;
    void Draw(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Shader& shader) const;
    void Draw(const std::shared_ptr<Model>& model, const Camera& camera, Shader& shader) const;
};

}
