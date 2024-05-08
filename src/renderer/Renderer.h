#pragma once

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"

namespace GoL {

class Renderer {
public:
    inline void Clear() const {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    template <Model M>
    void Draw(M& model, const Camera& camera, Shader& shader) const {
        shader.Bind();
        shader.SetUniformMat4f("Model", model.GetModelMatrix());
        shader.SetUniformMat4f("ProjectionView", camera.GetProjectionMatrix() * camera.GetViewMatrix());
        model.Draw();
    }
};



}
