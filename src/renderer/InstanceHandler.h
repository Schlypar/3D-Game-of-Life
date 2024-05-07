#pragma once

#include "Model.h"
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

public:
    std::vector<Instance> Instances;

    InstanceHandler(M model = M());
    ~InstanceHandler();

    void AddInstance(glm::vec3 position, glm::vec3 rotation, float scale);

    void Setup();
};

template <Model M>
InstanceHandler<M>::InstanceHandler(M model) {
    origin = model;
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

    Instances.push_back(std::move(instanceMatrix));
}

};