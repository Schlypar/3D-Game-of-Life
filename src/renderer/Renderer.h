#pragma once

#include <algorithm>
#include <bits/ranges_algo.h>
#include <ranges>
#include <vector>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

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

public:
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

    void ResetBatched() {
        this->batched.clear();
    }

    void DrawSubmitted(const Camera& camera) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        DrawSurfaces(this->batched, modelMatrix, projectionView);
    }

    void Draw(Model<Vertex>* model, const Camera& camera) const {
        std::vector<Surface<Vertex>> surfaces = model->GetSurfaces();
        glm::mat4 modelMatrix = model->GetModelMatrix();
        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        DrawSurfaces(surfaces, modelMatrix, projectionView);
    }

    void ConcatenateGeometry() {
        const auto projection = [](const Surface<Vertex>& s) -> int {
            return s.material->GetId();
        };
        const auto material = [projection](Surface<Vertex>& l, Surface<Vertex>& r) -> bool {
            auto left = projection(l);
            auto right = projection(r);
            return left == right;
        };
        const auto computeSurface = [](SurfaceBundle& sb) -> Surface<Vertex>& {
            auto& data = sb.surface.mesh->GetData();
            Vertex* vertexData = (Vertex*) data.bytes;
            for (int i = 0; i < data.size / sizeof(Vertex); i++) {
                Vertex& cur = vertexData[i];
                vertexData[i] = Vertex((sb.matrix * glm::vec4(cur.position, 1)), cur.color);
            }
            return sb.surface;
        };
        const auto concat = [](std::vector<Surface<Vertex>>& vec) -> Surface<Vertex> {
            Surface<Vertex>& res = vec[0];
            std::for_each(vec.begin() + 1, vec.end(), [&res](Surface<Vertex>& s) -> void {
                res += s;
            });
            return res;
        };

        auto computed = this->surfaces
                      | std::ranges::views::transform(computeSurface)
                      | std::ranges::to<std::vector<Surface<Vertex>>>();
        this->batched = std::move(computed);
        // std::ranges::sort(computed, {}, projection);

        // this->batched = computed
        //               | std::ranges::views::chunk_by(material)
        //               | std::ranges::to<std::vector<std::vector<Surface<Vertex>>>>()
        //               | std::ranges::views::transform(concat)
        //               | std::ranges::views::transform([](auto s) {s.mesh->Resize(); return s; })
        //               | std::ranges::to<std::vector<Surface<Vertex>>>();
    }

private:
    void DrawSurfaces(std::vector<Surface<Vertex>>& surfaces, const glm::mat4& modelMatrix, const glm::mat4& projectionView) const {
        for (Surface<Vertex>& surface : surfaces) {
            Mesh<Vertex>* mesh = surface.mesh;
            Material* material = surface.material;
            mesh->Bind();
            material->SetModel(modelMatrix);
            material->SetProjectionView(projectionView);
            material->Bind();
            glDrawArrays(surface.mode, 0, surface.vertexCount);
            mesh->Unbind();
        }
    }
};

}
