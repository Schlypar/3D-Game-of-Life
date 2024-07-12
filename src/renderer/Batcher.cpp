#include "Batcher.h"
#include <algorithm>
#include <ranges>

namespace GoL {

Batcher::Batcher(const Config& config)
    : config(config)
    , surfaces() {
}

void Batcher::SetConfig(const Config& config) {
    this->config = config;
}

void Batcher::Submit(Model<Vertex>* model) {
    auto modelSurfaces = model->GetSurfaces()
                       | std::ranges::views::transform([model](Surface<Vertex>& s) -> SurfaceBundle {
                             return SurfaceBundle { s, model->GetModelMatrix() };
                         })
                       | std::ranges::to<std::vector<SurfaceBundle>>();
    this->surfaces.insert(this->surfaces.end(), modelSurfaces.begin(), modelSurfaces.end());
}

void Batcher::Reset() {
    this->surfaces.clear();
}

std::vector<Surface<Vertex>> Batcher::ComputeBatches() {
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
    const auto optimize = [material, concat](std::vector<Surface<Vertex>>& surfaces) -> std::vector<Surface<Vertex>> {
        return surfaces
             | std::ranges::views::chunk_by(material)
             | std::ranges::to<std::vector<std::vector<Surface<Vertex>>>>()
             | std::ranges::views::transform(concat)
             | std::ranges::views::transform([](auto s) {s.mesh->Resize(); return s; })
             | std::ranges::to<std::vector<Surface<Vertex>>>();
    };

    auto copy = this->surfaces;
    auto computed = copy
                  | std::ranges::views::transform(computeSurface)
                  | std::ranges::to<std::vector<Surface<Vertex>>>();
    std::ranges::sort(computed, {}, projection);

    std::vector<std::vector<Surface<Vertex>>> batches;
    std::vector<Surface<Vertex>> batch;

    unsigned int vertexCount = 0;
    for (const auto& surface : computed) {
        if (vertexCount + surface.vertexCount <= this->config.maxVerticesPerBatch) {
            vertexCount += surface.vertexCount;
            batch.push_back(surface);
        } else {
            batches.push_back(std::move(batch));
            // batch = std::vector<Surface<Vertex>>();
            vertexCount = 0;
        }
    }
    if (!batch.empty() && vertexCount == 0) {
        batches.push_back(std::move(batch));
    }

    return batches
         | std::ranges::views::transform(optimize)
         | std::ranges::views::join
         | std::ranges::to<std::vector<Surface<Vertex>>>();
}

}
