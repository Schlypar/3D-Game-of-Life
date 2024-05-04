#pragma once

#include "../Model.h"
#include "Vertex.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace GoL {

class Prism : public Model {
// private:
public:
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    glm::vec3 position;
    glm::vec3 rotation;
    float scaleFactor;

public:
    Prism(glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         float scaleFactor = 1.0f
    )
        : vbo(nullptr, 0)
        , ibo(nullptr, 0)
        , position(position)
        , rotation(rotation)
        , scaleFactor(scaleFactor) {
        Vertex vertices[] = {

            // up
            { { 0.5f, 0.5f, 0.5f }, { 0, 0, 1 } },

            // bottom
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } },
            { { 0.5f, -0.5f, 0.5f }, { 0.9f, 0.0f, 0.3f } },
            { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.7f, 0.7f } },

        };
        unsigned int indices[] = {
            0, 1, 2,
            2,0,3,
            3,0,4,
            4,0,1,
            1,2,3,
            3,4,1,
        };

        vbo = VertexBuffer { vertices, sizeof(vertices) };

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

        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, (void*)0);
    }

    glm::mat4 GetModelMatrix() override {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));

        return modelMatrix;
    }
};

}