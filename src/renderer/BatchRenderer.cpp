#include "BatchRenderer.h"

#include "Meshes/Mesh.h"

namespace GoL {

BatchRenderer::BatchRenderer()
    : batcher(Batcher::Config { .maxThreads = 8 })
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

void BatchRenderer::Submit(Model<Vertex>* model) {
    this->batcher.Submit(model);
    this->changed = true;
}

void BatchRenderer::Reset() {
    this->batcher.Reset();
    this->changed = true;
}

void BatchRenderer::DrawSubmitted(const Camera& camera) {
    this->vertexArray.Bind();
    this->vertexBuffer.Bind();

    if (changed) {
        this->batched = std::move(this->batcher.ComputeBatches());
        this->changed = false;

        auto modelDataSize = ranges::accumulate(this->batched, 0, [](const unsigned int size, const Surface<Vertex>& s) -> int {
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
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
    DrawSurfaces(this->batched, modelMatrix, projectionView);

    this->vertexArray.Unbind();
    this->vertexBuffer.Unbind();
}

void BatchRenderer::DrawSurfaces(std::vector<Surface<Vertex>>& surfaces, glm::mat4& modelMatrix, const glm::mat4& projectionView) {
    unsigned int offset = 0;
    for (Surface<Vertex>& surface : surfaces) {
        Material* material = surface.material;
        material->SetProjectionView(projectionView);
        material->Bind(&modelMatrix);
        glDrawArrays(surface.mode, offset, surface.vertexCount);
        offset += surface.vertexCount;
    }
}

}
