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
        float scaling = 1.0f
    )
        : vbo(nullptr, 0)
        , ibo(nullptr, 0)
        , position_on_screen(position_on_screen)
        , scaling(scaling) {
        
        Vertex vertices[] = {
            { { 0.0f, 0.0f, 0.0f }, { 1, 1, 1 } },  // center
            { { 1.0f, 0.0f, 0.0f }, { 1, 0, 0 } },  // abscissa
            { { 0.0f, 0.0f, 0.0f }, { 1, 1, 1 } },  // center
            { { 0.0f, 1.0f, 0.0f }, { 0, 1, 0 } },  // ordinate
            { { 0.0f, 0.0f, 0.0f }, { 1, 1, 1 } },  // center
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
        ibo.Bind();
        vao.Bind();

        // glDrawArrays(GL_LINE_STRIP, 0, 2);
        // glDrawArrays(GL_LINE_STRIP, 2, 2);
        // glDrawArrays(GL_LINE_STRIP, 4, 2);
        glDrawElements(GL_LINES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    glm::mat4 GetModelMatrix() override {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, glm::vec3{ scaling });

        return modelMatrix;
    }
};

}