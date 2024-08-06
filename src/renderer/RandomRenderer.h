#ifndef RENDERER_H_
#define RENDERER_H_

#include "precompiled.h"

#include "Camera/Camera3D.h"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Models/Model.h"
#include "Models/Vertex.h"

namespace GoL {

class RandomRenderer {
private:
    struct SurfaceBundle {
        std::vector<Surface<Vertex>> surfaces;
        glm::mat4 matrix;
    };

private:
    std::vector<SurfaceBundle> surfaces;
    bool changed = true;

    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    unsigned int dataSize = 0;

public:
    RandomRenderer();

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
    void DrawSubmitted(const Camera3D* camera);

private:
    unsigned int DrawSurfaces(
            std::vector<Surface<Vertex>>& surfaces,
            const glm::mat4& modelMatrix,
            const glm::mat4& projectionView,
            unsigned int offset = 0
    );
};

}

#endif // RENDERER_H_
