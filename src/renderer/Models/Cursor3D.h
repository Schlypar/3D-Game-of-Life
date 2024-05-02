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
            // { { 0.0f, 0.0f, 0.0f }, { 1, 1, 1 } },  // center
            { { 1.0f, 0.0f, 0.0f }, { 1, 0, 0 } },  // abscissa
            { { 0.0f, 0.0f, 0.0f }, { 1, 0, 0 } },
            { { 0.0f, 1.0f, 0.0f }, { 0, 1, 0 } },  // ordinate
            { { 0.0f, 0.0f, 0.0f }, { 0, 1, 0 } },
            { { 0.0f, 0.0f, 1.0f }, { 0, 0, 1 } },  // applicata
            { { 0.0f, 0.0f, 0.0f }, { 0, 0, 1 } },
        };
        vbo = VertexBuffer { vertices, sizeof(vertices) };

        unsigned int indices[] = {
            0, 1,  // abscissa
            2, 3,  // ordinate
            4, 5   // applicata
        };
        ibo = IndexBuffer { indices, sizeof(indices) };

        GoL::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        vao.AddBuffer(vbo, layout);
    }

    void Draw() override {
        vbo.Bind();
        ibo.Bind();
        vao.Bind();

        glDrawArrays(GL_LINE_STRIP, 0, 2);
        glDrawArrays(GL_LINE_STRIP, 2, 2);
        glDrawArrays(GL_LINE_STRIP, 4, 2);
    }

    glm::mat4 GetModelMatrix() override {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
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
    auto position = camera.Position;
    auto rotation = glm::normalize(camera.Front);

    // modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
    // modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));

    shader.Bind();
    shader.SetUniformMat4f("Model", modelMatrix);
    shader.SetUniformMat4f("ProjectionView", glm::mat4(1.0f));
    model->Draw();
}

}