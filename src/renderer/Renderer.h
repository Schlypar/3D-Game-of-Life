#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Meshes/IndexedMesh.h"
#include "OldModel.h"

#include "Models/Model.h"
#include "Shader.h"

namespace GoL {

class Renderer {
public:
    inline void Clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template <OldModel M>
    void Draw(M& model, const Camera& camera, Shader& shader) const {
        shader.Bind();
        shader.SetUniformMat4f("Model", model.GetModelMatrix());
        shader.SetUniformMat4f("ProjectionView", camera.GetProjectionMatrix() * camera.GetViewMatrix());
        model.Draw();
    }

    void DrawTriangles(Model* model, const Camera& camera) const {
        std::vector<Surface> surfaces = model->GetSurfaces();
        glm::mat4 modelMatrix = model->GetModelMatrix();
        for (Surface& surface : surfaces) {
            Mesh* mesh = surface.mesh.get();
            Material* material = surface.material.get();
            material->SetModel(modelMatrix);
            material->SetProjectionView(camera.GetProjectionMatrix() * camera.GetViewMatrix());
            material->Bind();
            mesh->Bind();
            if (mesh->IsIndexed()) {
                IndexedMesh* indexedMesh = static_cast<IndexedMesh*>(mesh);
                const IndexBuffer::Id* indexBuffer = indexedMesh->GetIndexBuffer();
                // TODO: refactor IndexBuffer::GetCount out
                glDrawElements(GL_TRIANGLES, IndexBuffer::GetCount(*indexBuffer), GL_UNSIGNED_INT, IndexBuffer::GetOffset(*indexBuffer));
            } else {
                glDrawArrays(GL_TRIANGLES, 0, surface.vertexCount);
            }
        }
    }
};

}
