#pragma once

#include <algorithm>
#include <bits/ranges_algo.h>
#include <ranges>
#include <vector>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Batcher.h"
#include "Camera.h"

#include "Meshes/Mesh.h"
#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class Renderer {
private:
    struct SurfaceBundle {
        Surface<Vertex> surface;
        glm::mat4 matrix;
    };

private:
    std::vector<SurfaceBundle> surfaces;
    std::vector<Surface<Vertex>> batched;

    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    unsigned int dataSize = 0;

public:
    Renderer()
        : surfaces()
        , batched()
        , vertexArray()
        , vertexBuffer(nullptr, 0, GL_DYNAMIC_DRAW) {
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        vertexArray.AddBuffer(vertexBuffer, layout);
        this->vertexBuffer.Unbind();

        this->vertexArray.Unbind();
        this->vertexBuffer.Unbind();
    }

    inline void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Submit(Model<Vertex>* model) {
        auto modelSurfaces = model->GetSurfaces()
                           | std::ranges::views::transform([model](Surface<Vertex>& s) -> SurfaceBundle {
                                 return SurfaceBundle { s, model->GetModelMatrix() };
                             })
                           | std::ranges::to<std::vector<SurfaceBundle>>();
        this->surfaces.insert(this->surfaces.end(), modelSurfaces.begin(), modelSurfaces.end());
    }

    void SubmitBatches(std::vector<Surface<Vertex>>&& surfaces) {
        this->batched = std::move(surfaces);
    }

    void ResetBatched() {
        this->batched.clear();
    }

    void DrawSubmitted(const Camera& camera) {
        this->vertexArray.Bind();
        this->vertexBuffer.Bind();

        auto modelDataSize = std::ranges::fold_left(this->batched, 0, [](const unsigned int size, const Surface<Vertex>& s) -> int {
            return size + s.vertexCount * sizeof(Vertex);
        });
        if (modelDataSize > this->dataSize) {
            this->vertexBuffer.Realloc(modelDataSize, GL_DYNAMIC_DRAW);
            this->dataSize = modelDataSize;
        }
        unsigned int offset = 0;
        for (Surface<Vertex>& surface : this->batched) {
            Mesh<Vertex>* mesh = surface.mesh;

            auto& data = mesh->GetData();
            this->vertexBuffer.Write(data.bytes, data.size, offset * sizeof(Vertex));
            offset += surface.vertexCount;
        }

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        DrawSurfaces(this->batched, modelMatrix, projectionView);

        this->vertexArray.Unbind();
        this->vertexBuffer.Unbind();
    }

private:
    void DrawSurfaces(std::vector<Surface<Vertex>>& surfaces, const glm::mat4& modelMatrix, const glm::mat4& projectionView) {
        unsigned int offset = 0;
        for (Surface<Vertex>& surface : surfaces) {
            Material* material = surface.material;
            material->SetModel(modelMatrix);
            material->SetProjectionView(projectionView);
            material->Bind();
            glDrawArrays(surface.mode, offset, surface.vertexCount);
            offset += surface.vertexCount;
        }
    }
};

}
