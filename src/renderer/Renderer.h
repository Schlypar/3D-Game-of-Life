#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <bits/ranges_algo.h>
#include <ranges>
#include <vector>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "Meshes/IndexedMesh.h"

#include "Meshes/Mesh.h"
#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class Renderer {
private:
    struct SurfaceBundle {
        Surface surface;
        glm::mat4 matrix;
    };

private:
    std::vector<SurfaceBundle> surfaces;

public:
    inline void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        surfaces.clear();
    }

    void Submit(Model* model) {
        auto modelSurfaces = model->GetSurfaces()
                           | std::ranges::views::transform([model](Surface& s) -> SurfaceBundle {
                                 return SurfaceBundle { s, model->GetModelMatrix() };
                             })
                           | std::ranges::to<std::vector<SurfaceBundle>>();
        this->surfaces.insert(this->surfaces.end(), modelSurfaces.begin(), modelSurfaces.end());
    }

    void DrawSubmitted(const Camera& camera) {
        std::vector<Surface> optimized = this->ConcatenateGeometry();
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        DrawSurfaces(optimized, modelMatrix, projectionView);
    }

    void Draw(Model* model, const Camera& camera) const {
        std::vector<Surface> surfaces = model->GetSurfaces();
        glm::mat4 modelMatrix = model->GetModelMatrix();
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        DrawSurfaces(surfaces, modelMatrix, projectionView);
    }

private:
    void DrawSurfaces(std::vector<Surface>& surfaces, const glm::mat4& modelMatrix, const glm::mat4& projectionView) const {
        for (Surface& surface : surfaces) {
            Mesh* mesh = surface.mesh.get();
            Material* material = surface.material.get();
            mesh->Bind();
            material->SetModel(modelMatrix);
            material->SetProjectionView(projectionView);
            material->Bind();
            if (mesh->IsIndexed()) {
                // DEPRECATED
                IndexedMesh* indexedMesh = static_cast<IndexedMesh*>(mesh);
                const IndexBuffer::Id* indexBuffer = indexedMesh->GetIndexBuffer();
                glDrawElements(surface.mode, IndexBuffer::GetCount(*indexBuffer), GL_UNSIGNED_INT, IndexBuffer::GetOffset(*indexBuffer));
            } else {
                glDrawArrays(surface.mode, 0, surface.vertexCount);
            }
        }
    }

    std::vector<Surface> ConcatenateGeometry() {
        const auto projection = [](Surface& s) -> int {
            return s.material.get()->GetId();
        };
        const auto groupByMaterial = [projection](Surface& l, Surface& r) -> bool {
            auto left = projection(l);
            auto right = projection(r);
            return left == right;
        };
        const auto computeSurface = [](SurfaceBundle& sb) -> Surface& {
            auto& data = sb.surface.mesh->GetData();
            Vertex* vertexData = (Vertex*) data.bytes;
            for (int i = 0; i < data.size / sizeof(Vertex); i++) {
                Vertex& cur = vertexData[i];
                vertexData[i] = Vertex((sb.matrix * glm::vec4(cur.position, 1)), cur.color);
            }
            return sb.surface;
        };
        const auto concat = [](std::vector<Surface>& vec) -> Surface {
            std::cout << "LOOK\n";
            Surface& res = vec[0];
            std::for_each(vec.begin() + 1, vec.end(), [&res](Surface& s) -> void {
                res += s;
            });
            return res;
        };

        std::ranges::sort(this->surfaces, {}, [projection](SurfaceBundle& sb) -> int { return projection(sb.surface); });
        return this->surfaces
             | std::ranges::views::transform(computeSurface)
             | std::ranges::views::chunk_by(groupByMaterial)
             | std::ranges::to<std::vector<std::vector<Surface>>>()
             | std::ranges::views::transform(concat)
             | std::ranges::views::transform([](auto s) {s.mesh->Resize(); return s; })
             | std::ranges::to<std::vector<Surface>>();
    }
};

}
