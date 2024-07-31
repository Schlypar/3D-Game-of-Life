#pragma once

#include "precompiled.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Batcher.h"
#include "Camera.h"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class BatchRenderer {
private:
    struct SurfaceBundle {
        Surface<Vertex> surface;
        glm::mat4 matrix;
    };

private:
    Batcher batcher;
    std::vector<Surface<Vertex>> batched;
    bool changed = true;

    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    unsigned int dataSize = 0;

public:
    BatchRenderer();

    /**
    * @name Clear - Clears screen and depth buffer
    * @return void
    */
    inline void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /**
    * @name Submit - Submits model to be drawn on DrawSubmitted
    * @param model -  Pointer to the model to be drawn
    * @return void
    */
    void Submit(Model<Vertex>* model);

    /**
    * @name Reset - Forgets about all submitted models
    * @return void
    */
    void Reset();

    /**
    * @name DrawSubmitted - Draws all models that was submitted before
    * @param camera -  Perspective of the observer
    * @return void
    */
    void DrawSubmitted(const Camera& camera);

private:
    void DrawSurfaces(std::vector<Surface<Vertex>>& surfaces, const glm::mat4& modelMatrix, const glm::mat4& projectionView);
};

}
