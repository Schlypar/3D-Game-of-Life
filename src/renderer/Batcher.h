#ifndef BATCHER_H_
#define BATCHER_H_

#include "precompiled.h"

#include "Materials/Material.h"
#include "Meshes/Mesh.h"
#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class Batcher {
public:
    struct Config {
        unsigned int maxThreads = 8;
    };

private:
    struct SurfaceBundle {
        Surface<Vertex> surface;
        glm::mat4 matrix;
    };

private:
    Config config;
    std::vector<SurfaceBundle> surfaces;

    int maxVerticesPerBatch = 128;
    glm::mat4 batchTransform = glm::mat4 { 1.0f };

public:
    Batcher() = delete;
    Batcher(const Batcher& other) = delete;
    Batcher(Batcher&& other) = delete;
    Batcher(const Config& config = Config { .maxThreads = 8 });
    ~Batcher() = default;

    /**
    * @name SetConfig - Sets config to the given one
    * @param config -  New config
    * @return void
    */
    void SetConfig(const Config& config);

    /**
    * @name Submit - Stores Model for future optimizations
    * @param model -  Model to be submitted
    * @return void
    */
    void Submit(Model<Vertex>* model);

    /**
    * @name Reset - Resets the parameters
    * @return void
    */
    void Reset();

    /**
    * @name ComputeBatches - Optimizes geometry for reduction of draw calls
    * @return std::vector - collection concatenated Surfaces
    */
    std::vector<Surface<Vertex>> ComputeBatches();
};

}

#endif // BATCHER_H_
