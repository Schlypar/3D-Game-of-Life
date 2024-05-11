#pragma once

#include "IndexBuffer.h"
#include "Model.h"
#include "Models/Vertex.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "glad/gl.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

namespace GoL {

template <Model M>
class InstanceHandler {
private:
    VertexBuffer vbo;
    // VertexBufferLayout layout;
    M model;
    std::vector<glm::mat4> instances;
    GLuint vao;
    bool is_set_up;

    void setup() {
        GLuint VBO;
        glBindVertexArray(vao);

        model.GetVBO().Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), instances.data(), GL_STATIC_DRAW);

        int matrix_layout_loc = 2;
        for (int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(i + matrix_layout_loc);
            glVertexAttribPointer(i + matrix_layout_loc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4) + 2 * sizeof(glm::vec3)));
            glVertexAttribDivisorARB(i, 1);
        }

        glBindVertexArray(0);
        is_set_up = true;
    }

public:
    InstanceHandler(M model);
    ~InstanceHandler();

    void AddInstance(glm::vec3 position, glm::vec3 rotation, float scale);
    void Draw(int ibo, GLenum drawMode = GL_TRIANGLES);
};

template <Model M>
InstanceHandler<M>::InstanceHandler(M model) {
    is_set_up = false;
    this->model = model;
    glGenVertexArrays(1, &vao);
    // for (int i = 0; i < 4; i++) {
    //     layout.Push<float>(4);
    // }
}

template <Model M>
InstanceHandler<M>::~InstanceHandler() {
}

template <Model M>
void InstanceHandler<M>::AddInstance(glm::vec3 position, glm::vec3 rotation, float scale) {
    glm::mat4 instanceMatrix = glm::translate(glm::mat4(1.0f), position);
    instanceMatrix = glm::rotate(instanceMatrix, rotation.x, glm::vec3(1, 0, 0));
    instanceMatrix = glm::rotate(instanceMatrix, rotation.y, glm::vec3(0, 1, 0));
    instanceMatrix = glm::rotate(instanceMatrix, rotation.z, glm::vec3(0, 0, 1));
    instanceMatrix = glm::scale(instanceMatrix, glm::vec3(scale));

    instances.push_back(instanceMatrix);

    is_set_up = false;
}

template <Model M>
void InstanceHandler<M>::Draw(int ibo, GLenum drawMode) {
    if (!is_set_up) {
        setup();
    }

    glBindVertexArray(vao);
    // glDrawElementsInstanced(GL_TRIANGLES, IndexBuffer::GetCount(ibo), GL_UNSIGNED_INT, IndexBuffer::GetOffset(ibo), instances.size());
    glDrawArraysInstanced(drawMode, 0, 6, instances.size());
    glBindVertexArray(0);

}

// GLuint MBO, NUM_INSTANCES = instances.size(), MATRICES_LOCATION = 2;

// glGenBuffers(1, &(MBO));
// glBindBuffer(GL_ARRAY_BUFFER, MBO);
// glBufferData(GL_ARRAY_BUFFER, NUM_INSTANCES * 16 * sizeof(float), instances.data(), GL_DYNAMIC_DRAW);
// for (unsigned int i = 0; i < 4; i++) {
//     glEnableVertexAttribArray(MATRICES_LOCATION + i);
//     glVertexAttribPointer(MATRICES_LOCATION + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid*) (sizeof(GLfloat) * i * 4));
//     glVertexAttribDivisor(MATRICES_LOCATION + i, 1);
// }
// origin.GetVAO().Bind();
// origin.GetVBO().Bind();

};