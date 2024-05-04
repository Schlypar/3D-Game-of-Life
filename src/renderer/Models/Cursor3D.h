#pragma once

#include "../Model.h"
#include "../Renderer.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace GoL {

class Cursor3D : public Model {
private:
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    glm::vec3 position_on_screen;
    float scaling;

public:
    Cursor3D(
        glm::vec3 position_on_screen = glm::vec3{ 0.0f },
        float scaling = 0.1f
    )
        : vbo(nullptr, 0)
        , ibo(nullptr, 0)
        , position_on_screen(position_on_screen)
        , scaling(scaling) {
        
        Vertex vertices[] = {
            { { 0.0f, 0.0f, 0.0f }, { 1, 0, 0 } },  // center
            { { 1.0f, 0.0f, 0.0f }, { 1, 0, 0 } },  // abscissa
            { { 0.0f, 0.0f, 0.0f }, { 0, 1, 0 } },  // center
            { { 0.0f, 1.0f, 0.0f }, { 0, 1, 0 } },  // ordinate
            { { 0.0f, 0.0f, 0.0f }, { 0, 0, 1 } },  // center
            { { 0.0f, 0.0f, 1.0f }, { 0, 0, 1 } },  // applicata
        };
        vbo = VertexBuffer { vertices, sizeof(vertices) };

        unsigned int indices[] = {
            0, 1,  // abscissa
            2, 3,  // ordinate
            4, 5   // applicata
        };

        GoL::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        vao.AddBuffer(vbo, layout);

        ibo = IndexBuffer { indices, sizeof(indices) };
    }

    void Draw() override {
        vbo.Bind();
        vao.Bind();
        ibo.Bind();

        glDrawElements(GL_LINE_STRIP, ibo.GetCount(), GL_UNSIGNED_INT, (void*)0);

        // glDrawArrays(GL_LINES, 0, 2);
        // glDrawArrays(GL_LINES, 2, 2);
        // glDrawArrays(GL_LINES, 4, 2);

        // glDrawArrays(GL_LINE_STRIP, 0, 2);
        // glDrawArrays(GL_LINE_STRIP, 2, 2);
        // glDrawArrays(GL_LINE_STRIP, 4, 2);
        // glDrawElements(GL_LINES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    glm::mat4 GetModelMatrix() override {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position_on_screen);
        modelMatrix = glm::scale(modelMatrix, glm::vec3{ scaling });

        return modelMatrix;
    }
};

class Cursor3DRenderer : public Renderer {
public:
    void Draw(const std::shared_ptr<Model>& model, const Camera& camera, Shader& shader) const;
};

void Cursor3DRenderer::Draw(const std::shared_ptr<Model>& model, const Camera& camera, Shader& shader) const {
    auto modelMatrix = model->GetModelMatrix();
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.Yaw), glm::vec3(modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2]));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(camera.Pitch), glm::vec3(modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2]));

    // shader.Bind();
    shader.SetUniformMat4f("Model", modelMatrix);
    shader.SetUniformMat4f("ProjectionView", glm::mat4(1.0f));
    model->Draw();
}

}