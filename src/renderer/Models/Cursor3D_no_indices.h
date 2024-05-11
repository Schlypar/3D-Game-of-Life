#pragma once

#include "../Model.h"
#include "../Renderer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glad/gl.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace GoL {

class Cursor3D_no_i {
private:
    VertexArray vao;
    VertexBuffer vbo;

    glm::vec3 position_on_screen;
    float scaling;

    constexpr static const Vertex vertices[] = {
        { { 0.0f, 0.0f, 0.0f }, { 1, 0, 0 } }, // center
        { { 1.0f, 0.0f, 0.0f }, { 1, 0, 0 } }, // abscissa
        { { 0.0f, 0.0f, 0.0f }, { 0, 1, 0 } }, // center
        { { 0.0f, 1.0f, 0.0f }, { 0, 1, 0 } }, // ordinate
        { { 0.0f, 0.0f, 0.0f }, { 0, 0, 1 } }, // center
        { { 0.0f, 0.0f, 1.0f }, { 0, 0, 1 } }, // applicata
    };

public:
    Cursor3D_no_i(
            glm::vec3 position_on_screen = glm::vec3 { 0.0f },
            float scaling = 0.1f
    )
        : vbo(nullptr, 0)
        , position_on_screen(position_on_screen)
        , scaling(scaling) {

        vbo = VertexBuffer { vertices, sizeof(vertices) };

        GoL::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        vao.AddBuffer(std::vector<VertexBuffer> { vbo }, layout);
    }

    void Draw() {
        vbo.Bind();
        vao.Bind();

        // glDrawElements(GL_LINE_STRIP, IndexBuffer::GetCount(*ibo), GL_UNSIGNED_INT, IndexBuffer::GetOffset(*ibo));
        glDrawArrays(GL_LINE_STRIP, 0, 6);
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position_on_screen);
        modelMatrix = glm::scale(modelMatrix, glm::vec3 { scaling });

        return modelMatrix;
    }

    void SetScaleFactor(float scale) {
        scaling = scale;
    }

    constexpr std::pair<const Vertex*, int> GetVerticies() {
        return std::pair<const Vertex*, int>(vertices, 6);
    }

    VertexArray& GetVAO() {
        return vao;
    }

    VertexBuffer& GetVBO() {
        return vbo;
    }

public:
    template <Model M>
    static void CustomDraw(M& model, const Camera& camera, Shader& shader) {
        auto modelMatrix = model.GetModelMatrix();
        modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.Yaw), glm::vec3(modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2]));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.Pitch), glm::vec3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2]));

        shader.Bind();
        shader.SetUniformMat4f("Model", modelMatrix);
        shader.SetUniformMat4f("ProjectionView", glm::mat4(1.0f));
        model.Draw();
    }
};

}