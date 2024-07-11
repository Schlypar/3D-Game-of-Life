#pragma once

#include <cassert>
#include <string>

#include "MaterialLibrary.h"
#include "Shader.h"

namespace GoL {

class Material {
protected:
    Shader shader;
    std::string name;
    const glm::mat4* model;
    const glm::mat4* projectionView;

public:
    Material(const Shader& shader, const std::string& name)
        : shader(shader)
        , name(name)
        , model(nullptr)
        , projectionView(nullptr) {
    }

    Material(Material* other)
        : shader(other->shader)
        , name(other->name)
        , model(other->model)
        , projectionView(other->projectionView) {
    }

    Material(const Material& other) = delete;
    Material(Material&& other) = delete;

    virtual ~Material() = default;

    unsigned int GetId() const {
        return shader.GetShaderId();
    }

    const std::string& GetName() const {
        return name;
    }

    void SetModel(const glm::mat4& model) {
        this->model = &model;
    }

    void SetProjectionView(const glm::mat4& projectionView) {
        this->projectionView = &projectionView;
    }

    virtual void Bind() {
        BaseBind();
    }

protected:
    virtual void BaseBind() {
        assert(model != nullptr && projectionView != nullptr);
        shader.Bind();
        shader.SetUniformMat4f("Model", *model);
        shader.SetUniformMat4f("ProjectionView", *projectionView);
    }
};

}
