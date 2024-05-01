#include "Renderer.h"

namespace GoL {

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Shader& shader) const {
    shader.Bind();
    vertexBuffer.Bind();
    indexBuffer.Bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const std::shared_ptr<Model>& model, const Camera& camera, Shader& shader) const {
    shader.Bind();
    shader.SetUniformMat4f("Model", model->GetModelMatrix());
    shader.SetUniformMat4f("ProjectionView", camera.GetProjectionMatrix() * camera.GetViewMatrix());
    model->Draw();
}

}
