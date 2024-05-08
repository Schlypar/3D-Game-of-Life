#pragma once

#include "../Model.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace GoL {

class Prism {
private:
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer::Id* ibo;

    glm::vec3 position;
    glm::vec3 rotation;
    float scaleFactor;

    constexpr static const Vertex vertices[] = {

        // up
        { { 0.5f, 0.5f, 0.5f }, { 0, 0, 1 } },

        // bottom
        { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
        { { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } },
        { { 0.5f, -0.5f, 0.5f }, { 0.9f, 0.0f, 0.3f } },
        { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.7f, 0.7f } },

    };

public:
    Prism(glm::vec3 position = glm::vec3(0.0f),
          glm::vec3 rotation = glm::vec3(0.0f),
          float scaleFactor = 1.0f
    )
        : vbo(nullptr, 0)
        , ibo(nullptr)
        , position(position)
        , rotation(rotation)
        , scaleFactor(scaleFactor) {

        vbo = VertexBuffer { vertices, sizeof(vertices) };

        GoL::VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        vao.AddBuffer(vbo, layout);
    }

    void BindIndices() {
        unsigned int indices[] = {
            0,
            1,
            2,
            2,
            0,
            3,
            3,
            0,
            4,
            4,
            0,
            1,
            1,
            2,
            3,
            3,
            4,
            1,
        };

        ibo = new IndexBuffer::Id;
        *ibo = IndexBuffer::Register(indices, sizeof(indices) / sizeof(unsigned int));
    }

    void Draw() {
        vbo.Bind();
        vao.Bind();

        glDrawElements(GL_TRIANGLES, IndexBuffer::GetCount(*ibo), GL_UNSIGNED_INT, IndexBuffer::GetOffset(*ibo));
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));

        return modelMatrix;
    }

    std::pair<const Vertex*, int> GetVerticies() {
        return std::pair<const Vertex*, int>(vertices, 5);
    }

    ~Prism() {
        if (ibo != nullptr) {
            delete ibo;
        }
    }
};

}