#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Meshes/IndexedMesh.h"

#include "Models/Model.h"

namespace GoL {

class Renderer {
public:
    inline void Clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Draw(Model* model, const Camera& camera) const {
        std::vector<Surface> surfaces = model->GetSurfaces();
        glm::mat4 modelMatrix = model->GetModelMatrix();
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        for (Surface& surface : surfaces) {
            Mesh* mesh = surface.mesh.get();
            Material* material = surface.material.get();
            material->SetModel(modelMatrix);
            material->SetProjectionView(projectionView);
            material->Bind();
            mesh->Bind();
            if (mesh->IsIndexed()) {
                IndexedMesh* indexedMesh = static_cast<IndexedMesh*>(mesh);
                const IndexBuffer::Id* indexBuffer = indexedMesh->GetIndexBuffer();
                // TODO: refactor IndexBuffer::GetCount out
                glDrawElements(surface.mode, IndexBuffer::GetCount(*indexBuffer), GL_UNSIGNED_INT, IndexBuffer::GetOffset(*indexBuffer));
            } else {
                glDrawArrays(surface.mode, 0, surface.vertexCount);
            }
        }
    }
};

}
