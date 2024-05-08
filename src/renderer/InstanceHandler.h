#pragma once

#include "Model.h"
#include "VertexBufferLayout.h"
#include "glm/ext/matrix_transform.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace GoL {

template <Model M>
class InstanceHandler {
private:
    struct Instance {
        glm::mat4 instanceMatrix;
    };

private:
    M origin;
    VertexBufferLayout layout;
    std::vector<Instance> instances;

public:
    InstanceHandler(M model = M());
    ~InstanceHandler();

    void AddInstance(glm::vec3 position, glm::vec3 rotation, float scale);
    void Draw();
};

template <Model M>
InstanceHandler<M>::InstanceHandler(M model) {
    origin = model;
    layout = model.GetLayout();
    for (int i = 0; i < 4; i++) {
        layout.Push<float>(4);
    }
}

template <Model M>
InstanceHandler<M>::~InstanceHandler() {
}

template <Model M>
void InstanceHandler<M>::AddInstance(glm::vec3 position, glm::vec3 rotation, float scale) {
    glm::mat4 instanceMatrix = glm::translate(origin.GetModelMatrix(), position);
    instanceMatrix = glm::rotate(instanceMatrix, rotation.x, glm::vec3(1, 0, 0));
    instanceMatrix = glm::rotate(instanceMatrix, rotation.y, glm::vec3(0, 1, 0));
    instanceMatrix = glm::rotate(instanceMatrix, rotation.z, glm::vec3(0, 0, 1));
    instanceMatrix = glm::scale(instanceMatrix, glm::vec3(scale));

    instances.push_back(std::move(instanceMatrix));
}

template <Model M>
void InstanceHandler<M>::Draw() {
    
}

};