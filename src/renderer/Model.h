#pragma once

#include <glm/glm.hpp>

namespace GoL {

class Model {
public:
    virtual ~Model() = default;

    virtual void Draw() = 0;
    virtual glm::mat4 GetModelMatrix() = 0;
};

}