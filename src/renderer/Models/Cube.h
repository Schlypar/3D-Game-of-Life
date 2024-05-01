#pragma once

#include "../Model.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

namespace GoL {

class Cube : public Model {
private:
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    glm::vec3 position;
    glm::vec3 rotation;
    float scaleFactor;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
    };

public:
    Cube(glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 rotation = glm::vec3(0.0f),
         float scaleFactor = 1.0f
    )
        : vbo(nullptr, 0)
        , ibo(nullptr, 0)
        , position(position)
        , rotation(rotation)
        , scaleFactor(scaleFactor) {
        Vertex vertices[] = {
            // front
            { { -0.5f, -0.5, -0.5f }, { 1, 0, 0 } },
            { { 0.5f, -0.5f, -0.5f }, { 1, 0, 0 } },
            { { 0.5f, 0.5f, -0.5f }, { 1, 0, 0 } },
            { { -0.5f, 0.5f, -0.5f }, { 1, 0, 0 } },

            // back
            { { -0.5f, -0.5, 0.5f }, { 0, 1, 0 } },
            { { 0.5f, -0.5f, 0.5f }, { 0, 1, 0 } },
            { { 0.5f, 0.5f, 0.5f }, { 0, 1, 0 } },
            { { -0.5f, 0.5f, 0.5f }, { 0, 1, 0 } },

            // up
            { { -0.5f, 0.5f, -0.5f }, { 0, 0, 1 } },
            { { 0.5f, 0.5f, -0.5f }, { 0, 0, 1 } },
            { { 0.5f, 0.5f, 0.5f }, { 0, 0, 1 } },
            { { -0.5f, 0.5f, 0.5f }, { 0, 0, 1 } },

            // bottom
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.7f, 0.7f } },
            { { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.7f, 0.7f } },
            { { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.7f, 0.7f } },
            { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.7f, 0.7f } },

            // right
            { { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f  } },
            { { 0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f  } },
            { { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f  } },
            { { 0.5f, -0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f  } },

            // left
            { { -0.5f, -0.5f, -0.5f }, { 0.9f, 0.0f, 0.3f } },
            { { -0.5f, 0.5f, -0.5f }, { 0.9f, 0.0f, 0.3f } },
            { { -0.5f, 0.5f, 0.5f }, { 0.9f, 0.0f, 0.3f } },
            { { -0.5f, -0.5f, 0.5f }, { 0.9f, 0.0f, 0.3f } },
        };
        unsigned int indices[] = {
            //front
            0, 1, 2,
            2, 3, 0,

            // back
            4, 5, 6,
            6, 7, 4,

            // up
            8, 9, 10,
            10, 11, 8,

            // bottom
            12, 13, 14,
            14, 15, 12,

            // right
            16, 17, 18,
            18, 19, 16,

            // left
            20, 21, 22,
            22, 23, 20
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

        glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
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