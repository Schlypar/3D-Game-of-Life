#include "Batcher.h"

namespace GoL {

Batcher::Batcher(const Config& config)
    : config(config)
    , surfaces() {
}

void Batcher::SetConfig(const Config& config) {
    this->config = config;
}

void Batcher::Submit(Model<Vertex>* model) {
    auto rawSurfaces = model->GetSurfaces();
    auto modelSurfaces = rawSurfaces
                       | ranges::views::transform([model](Surface<Vertex>& s) -> SurfaceBundle {
                             return SurfaceBundle { s, model->GetModelMatrix() };
                         })
                       | ranges::to<std::vector<SurfaceBundle>>();
    this->surfaces.insert(this->surfaces.end(), modelSurfaces.begin(), modelSurfaces.end());
}

void Batcher::Reset() {
    this->surfaces.clear();
}

std::vector<Surface<Vertex>> Batcher::ComputeBatches() {
    std::mutex mutex;
    std::vector<Surface<Vertex>> result;

    const auto projectionToId = [](const Surface<Vertex>& s) -> int {
        return s.material->GetId();
    };
    const auto materialAndMode = [](const Surface<Vertex>& left, const Surface<Vertex>& right) -> bool {
        return (left.mode == right.mode) && (*left.material == right.material);
    };
    const auto computeSurface = [](const SurfaceBundle& sb) -> const Surface<Vertex>& {
        auto& data = sb.surface.mesh->GetData();
        Vertex* vertexData = (Vertex*) data.bytes;
        for (int i = 0; i < data.size / sizeof(Vertex); i++) {
            Vertex& cur = vertexData[i];
            vertexData[i] = Vertex((sb.matrix * glm::vec4(cur.position, 1)), cur.color);
        }
        return sb.surface;
    };
    const auto concat = [](const auto& vec) -> Surface<Vertex> {
        Surface<Vertex> res = vec[0];
        std::for_each(vec.begin() + 1, vec.end(), [&res](const Surface<Vertex>& s) -> void {
            res += s;
        });
        return res;
    };

    const auto optimize = [computeSurface, materialAndMode, concat, &result, &mutex](const std::vector<SurfaceBundle>& vec) {
        auto transformed = vec
                         | ranges::views::transform(computeSurface)
                         | ranges::to<std::vector<Surface<Vertex>>>();
        auto intermediate = transformed
                          | ranges::views::chunk_by(materialAndMode)
                          | ranges::to<std::vector<std::vector<Surface<Vertex>>>>();
        transformed = intermediate
                    | ranges::views::transform(concat)
                    | ranges::to<std::vector<Surface<Vertex>>>();

        mutex.lock();
        result.insert(result.end(), transformed.begin(), transformed.end());
        mutex.unlock();
    };

    this->maxVerticesPerBatch = (ranges::accumulate(this->surfaces, 0, [](int acc, const auto& sb) -> int {
                                     return acc + sb.surface.vertexCount;
                                 })
                                 / this->config.maxThreads);

    std::ranges::sort(this->surfaces, {}, [projectionToId](const auto& sb) -> int { return projectionToId(sb.surface); });

    std::vector<std::vector<SurfaceBundle>> batches;
    std::vector<SurfaceBundle> batch;

    unsigned int vertexCount = 0;
    for (const auto& sb : this->surfaces) {
        if (vertexCount + sb.surface.vertexCount <= this->maxVerticesPerBatch) {
            vertexCount += sb.surface.vertexCount;
            batch.push_back(sb);
        } else {
            batches.push_back(std::move(batch));
            batch = { sb };
            vertexCount = 0;
        }
    }
    batches.push_back(std::move(batch));

    std::thread* workers = new std::thread[this->config.maxThreads];
    int threadComputedTimes = 0;
    while (batches.size() > threadComputedTimes * this->config.maxThreads) {
        if (batches.size() - threadComputedTimes * this->config.maxThreads >= this->config.maxThreads) {
            for (int i = 0; i < this->config.maxThreads; i++) {
                auto& current = batches[i + threadComputedTimes * this->config.maxThreads];
                workers[i] = std::thread(optimize, current);
            }
            for (int i = 0; i < this->config.maxThreads; i++) {
                workers[i].join();
            }
            threadComputedTimes++;
        } else {
            for (int i = 0; i < batches.size() - threadComputedTimes * this->config.maxThreads; i++) {
                auto& current = batches[i + threadComputedTimes * this->config.maxThreads];
                workers[i] = std::thread(optimize, current);
            }
            for (int i = 0; i < batches.size() - threadComputedTimes * this->config.maxThreads; i++) {
                workers[i].join();
            }
            threadComputedTimes++;
        }
    }
    delete[] workers;

    return result;
}
}
