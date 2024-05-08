#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "IndexBuffer.h"
#include "InstanceHandler.h"
#include "Model.h"
#include "Shader.h"

namespace GoL {

class Renderer {

public:
    static inline void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template <Model M>
    static void DrawBasic(M& model, const Camera& camera, Shader& shader) {
        shader.Bind();
        shader.SetUniformMat4f("Model", model.GetModelMatrix());
        shader.SetUniformMat4f("ProjectionView", camera.GetProjectionMatrix() * camera.GetViewMatrix());
        model.Draw();
    }

    template <Model M, void (*CustomDraw)(M&, const Camera&, Shader&)>
    static void DrawCustom(M& model, const Camera& camera, Shader& shader) {
        CustomDraw(model, camera, shader);
    }

    template <Model M>
    static void DrawInstanced(InstanceHandler<M>& instancies, const Camera& camera, Shader& shader) {
        shader.Bind();
        shader.SetUniformMat4f("ProjectionView", camera.GetProjectionMatrix() * camera.GetViewMatrix());
        instancies.Draw();
    }
};

}
