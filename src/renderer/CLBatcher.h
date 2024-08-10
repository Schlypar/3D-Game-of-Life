#pragma once

#include <list>
#include <vector>
#include <tuple>

#include "precompiled.h"
#include "Camera.h"

#include "Materials/Material.h"
#include "Meshes/Mesh.h"
#include "Models/Model.h"
#include "Models/Vertex.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "CL/Device.h"
#include "CL/Platform.h"
#include "CLHandler.h"

namespace GoL {

class CLBatcher {
public:
    struct Config {
        CL::Platform platform;
        CL::Device device;
        bool has_cl_khr_gl_sharing;
    };

private:
    using Batch = std::list<Surface<Vertex>>;

private:
    Config config;
    CLHandler cl;
    VertexArray vao;
    VertexBuffer vbo;
    std::size_t vboSize;

    std::list<Batch> batches;
    std::vector<CLHandler::clVertex> vertexBuffer;
    std::vector<glm::mat4> modelMatrices;
    std::vector <std::size_t> batchOffsetsInBuffer;

    std::size_t totalSurfaces;
    std::size_t totalVertexCount;

public:
    CLBatcher() = delete;
    CLBatcher(const CLBatcher& other) = delete;
    CLBatcher(CLBatcher&& other) = delete;
    CLBatcher(Config config);
    ~CLBatcher() = default;

    inline void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /**
    * @name SetConfig - Sets config to the given one
    * @param config -  New config
    * @return void
    */
    void SetConfig(Config& config);

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

   void DrawSubmitted(const Camera& camera);

private:
    std::vector<Vertex> ComputeBatches();
    std::size_t AddMatrix(glm::mat4* matrix);
    std::size_t GetMatrixIndex(glm::mat4* matrix);
    void InsertOrdered(Surface<Vertex> surface);
    std::vector<CLHandler::clVertex>& ConcatAndMapSurfaces();
};

}
