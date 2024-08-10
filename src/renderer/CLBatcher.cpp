#include "CLBatcher.h"

#include <algorithm>

namespace GoL {

CLBatcher::CLBatcher(Config config)
    : config(config)
    , totalVertexCount(0)
    , totalSurfaces(0)
    , vao()
    , vbo(nullptr, 0, GL_DYNAMIC_DRAW)
    , vboSize(0) {

    cl = CLHandler(config.platform, config.device);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);

    this->vao.AddBuffer(vbo, layout);
    this->vbo.Unbind();

    this->vao.Unbind();
    this->vbo.Unbind();
}

void CLBatcher::SetConfig(Config& config) {
    this->config = config;
    cl = CLHandler(config.platform, config.device);
}

void CLBatcher::Submit(Model<Vertex>* model) {
    auto matIndex = this->AddMatrix(&model->GetModelMatrix());

    auto surfs = model->GetSurfaces();
    for (auto& s : surfs) {
        s.SetModel((glm::mat4*) matIndex);
        this->totalVertexCount += s.vertexCount;
        this->InsertOrdered(s);
        this->totalSurfaces++;
    }
}

// not recommended?
void CLBatcher::Reset() {
    this->batches.clear();
    this->vertexBuffer.clear();
    this->modelMatrices.clear();
    this->batchOffsetsInBuffer.clear();
    this->totalSurfaces = 0;
    this->totalVertexCount = 0;
}

std::vector<Vertex> CLBatcher::ComputeBatches() {
    auto& vBuffer = this->ConcatAndMapSurfaces();

    std::size_t neededVboSize = std::max((std::size_t) 192 * 1024, this->vertexBuffer.size() * sizeof(Vertex));
    if (neededVboSize > this->vboSize) {
        this->vbo.Realloc(neededVboSize, GL_DYNAMIC_DRAW);
        this->vboSize = neededVboSize;
    }

    return cl.CalculateModelMatrices(vBuffer, this->modelMatrices, this->vbo, this->config.has_cl_khr_gl_sharing ? this->vboSize : 0);
}

void CLBatcher::DrawSubmitted(const Camera& camera) {
    auto vboData = this->ComputeBatches();

    this->vao.Bind();
    this->vbo.Bind();

    if (vboData.size()) {
        vbo.Write(vboData.data(), vboData.size() * sizeof(Vertex), 0);
    }

    glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();

    auto batch = this->batches.begin();
    auto& offsets = this->batchOffsetsInBuffer;
    for (int i = 0; i < offsets.size() - 1; i++, batch++) {
        batch->front().material->SetProjectionView(projectionView);
        batch->front().material->Bind(this->modelMatrices.data() + (std::size_t) batch->front().GetModel());
        glDrawArrays(batch->front().mode, offsets[i], offsets[i + 1] - offsets[i]);
    }
}

std::size_t CLBatcher::AddMatrix(glm::mat4* matrix) {
    auto sameMatrix = std::find_if(this->modelMatrices.begin(), this->modelMatrices.end(), [matrix](glm::mat4& mat) -> bool { return *matrix == mat; });
    if (sameMatrix == this->modelMatrices.end()) {
        this->modelMatrices.push_back(*matrix);
        return this->modelMatrices.size() - 1;
    }

    return std::distance(this->modelMatrices.begin(), sameMatrix);
}

void CLBatcher::InsertOrdered(Surface<Vertex> surface) {
    auto groupCtiteria = [&surface](Batch& batch) -> bool {
        if (batch.size() == 0) {
            return true;
        }

        auto& s = batch.front();
        return surface.material->GetId() == s.material->GetId() && surface.mode == s.mode && surface.mesh->GetLayout() == s.mesh->GetLayout() && surface.material == s.material;
    };

    auto correctBatch = std::find_if(
            this->batches.begin(),
            this->batches.end(),
            groupCtiteria
    );

    if (correctBatch == this->batches.end()) {
        this->batches.push_back({ surface });
    } else {
        correctBatch->push_back(surface);
    }
}

// before calculations
std::vector<CLHandler::clVertex>& CLBatcher::ConcatAndMapSurfaces() {
    if (this->totalVertexCount > this->vertexBuffer.size()) {
        this->vertexBuffer.resize(this->totalVertexCount);
    }

    if (this->batches.size() + 1 > this->batchOffsetsInBuffer.size()) {
        this->batchOffsetsInBuffer.resize(this->batches.size() + 1);
    }

    std::size_t offset = 0, surfaceIndex = 0, batchIndex = 0;
    for (auto& batch : this->batches) {
        this->batchOffsetsInBuffer[batchIndex++] = offset;

        for (auto& s : batch) {
            auto& vertices = s.mesh->GetData();
            std::size_t matrixIndex = this->GetMatrixIndex(s.GetModel());
            for (int i = 0; i < vertices.size / sizeof(Vertex); i++) {
                this->vertexBuffer[offset + i] = { vertices.bytes[i], (cl_float) matrixIndex };
            }

            offset += vertices.size / sizeof(Vertex);
        }
    }

    this->batchOffsetsInBuffer[batchIndex] = offset;

    return this->vertexBuffer;
}

std::size_t CLBatcher::GetMatrixIndex(glm::mat4* matrix) {
    return (std::size_t) matrix;
}

}
