#ifndef BATCHER_H_
#define BATCHER_H_

#include <algorithm>
#include <bits/ranges_algo.h>
#include <functional>
#include <ranges>
#include <vector>

#include "Materials/Material.h"
#include "Meshes/Mesh.h"
#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class Batcher {
public:
    struct Config {
        int maxVerticesPerBatch = 128;
        unsigned int maxThreads = 8;
        glm::mat4 batchTransform = glm::mat4 { 1.0f };
    };

private:
    struct SurfaceBundle {
        Surface<Vertex> surface;
        glm::mat4 matrix;
    };

private:
    Config config;
    std::vector<SurfaceBundle> surfaces;

public:
    Batcher() = delete;
    Batcher(const Batcher& other) = delete;
    Batcher(Batcher&& other) = delete;
    Batcher(const Config& config = Config { 128, 8, glm::mat4 { 1.0f } });
    ~Batcher() = default;

    void SetConfig(const Config& config);

    void Submit(Model<Vertex>* model);
    void Reset();
    std::vector<Surface<Vertex>> ComputeBatches();

    unsigned int GetSize() {
        return this->surfaces.size();
    }
};

}

#endif // BATCHER_H_
