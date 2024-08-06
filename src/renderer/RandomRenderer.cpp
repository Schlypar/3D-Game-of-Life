#include "RandomRenderer.h"

namespace GoL {

RandomRenderer::RandomRenderer()
    : surfaces()
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

void RandomRenderer::Submit(Model<Vertex>* model) {
    this->surfaces.push_back({ model->GetSurfaces(), model->GetModelMatrix() });
    this->changed = true;
}

void RandomRenderer::Reset() {
    this->surfaces.clear();
    this->changed = true;
}

void RandomRenderer::DrawSubmitted(const Camera3D* camera) {
    this->vertexArray.Bind();
    this->vertexBuffer.Bind();

    if (changed) {
        this->changed = false;
        auto modelDataSize = ranges::accumulate(this->surfaces, 0, [](unsigned int size, const SurfaceBundle& sb) -> int {
            for (auto& surface : sb.surfaces) {
                size += surface.vertexCount * sizeof(Vertex);
            }
            return size;
        });
        if (modelDataSize > this->dataSize) {
            this->vertexBuffer.Realloc(modelDataSize, GL_DYNAMIC_DRAW);
            this->dataSize = modelDataSize;
        }
        unsigned int offset = 0;
        for (SurfaceBundle& sb : this->surfaces) {
            for (Surface<Vertex>& surface : sb.surfaces) {
                Mesh<Vertex>* mesh = surface.mesh;

                auto& data = mesh->GetData();
                this->vertexBuffer.Write(data.bytes, data.size, offset * sizeof(Vertex));
                offset += surface.vertexCount;
            }
        }
    }

    unsigned int offset = 0;
    for (SurfaceBundle& sb : this->surfaces) {
        glm::mat4 modelMatrix = sb.matrix;
        glm::mat4 projectionView = camera->GetProjectionMatrix() * camera->GetViewMatrix();
        offset = DrawSurfaces(sb.surfaces, modelMatrix, projectionView, offset);
    }

    this->vertexArray.Unbind();
    this->vertexBuffer.Unbind();
}

unsigned int RandomRenderer::DrawSurfaces(
        std::vector<Surface<Vertex>>& surfaces,
        const glm::mat4& modelMatrix,
        const glm::mat4& projectionView,
        unsigned int offset
) {
    for (Surface<Vertex>& surface : surfaces) {
        Material* material = surface.material;
        material->SetModel(modelMatrix);
        material->SetProjectionView(projectionView);
        material->Bind();
        glDrawArrays(surface.mode, offset, surface.vertexCount);
        offset += surface.vertexCount;
    }

    return offset;
}

}
