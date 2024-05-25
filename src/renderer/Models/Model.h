#pragma once

#include <memory>

#include "Materials/Material.h"
#include "Meshes/Mesh.h"

namespace GoL {

class Model {
protected:
    template <typename T>
    using Ref = std::shared_ptr<T>;

    using Surface = std::pair<Ref<Material>, Ref<Mesh>>;

protected:
    glm::mat4 model;
    std::vector<Surface> surfaces;

protected:
    Model(const glm::mat4& model, const std::vector<Surface>& surfaces)
        : model(model)
        , surfaces(surfaces) {
    }

public:
    virtual ~Model() = default;

    const glm::mat4& GetModelMatrix() const {
        return model;
    }
};
}