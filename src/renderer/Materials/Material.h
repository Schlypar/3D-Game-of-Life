#pragma once

#include "precompiled.h"

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

    virtual bool operator==(Material* other) = 0;

    virtual ~Material() = default;

    /**
    * @name GetId - Gives id of undelying Shader
    * @return unsigned int - id of Shader used
    */
    unsigned int GetId() const {
        return shader.GetShaderId();
    }

    /**
    * @name GetName - Gives name of Material
    * @return std::string - name of Material
    */
    const std::string& GetName() const {
        return name;
    }

    /**
    * @name SetModel - Sets Model on which this Material will apply
    * @param model -  Pointer to the model
    * @return void
    */
    void SetModel(const glm::mat4& model) {
        this->model = &model;
    }

    /**
    * @name SetProjectionView - Uploads matrix into the GPU program
    * @param projectionView - Result of multiplication of projection and View (proj * view)
    * @return void
    */
    void SetProjectionView(const glm::mat4& projectionView) {
        this->projectionView = &projectionView;
    }

    /**
    * @name Bind - Loads all matrices into GPU program as well as Model matrix of the Model set with SetModel
    * @return void
    */
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

struct MaterialPointer {
    Material* material;
    bool hasOwnership;

    MaterialPointer()
        : material(nullptr)
        , hasOwnership(false) {
    }

    MaterialPointer(Material* material, bool hasOwnership)
        : material(material)
        , hasOwnership(hasOwnership) {
    }

    MaterialPointer(const MaterialPointer& other)
        : material(other.material)
        , hasOwnership(false) {
    }

    MaterialPointer(MaterialPointer&& other)
        : material(other.material)
        , hasOwnership(other.hasOwnership) {
        other.material = nullptr;
        other.hasOwnership = false;
    }

    ~MaterialPointer() {
        if (hasOwnership) {
            delete material;
        }
    }

    MaterialPointer& operator=(const MaterialPointer& other) {
        if (this == &other) {
            return *this;
        }

        this->material = other.material;
        this->hasOwnership = false;

        return *this;
    }

    MaterialPointer& operator=(MaterialPointer&& other) {
        if (this == &other) {
            return *this;
        }

        this->material = other.material;
        this->hasOwnership = other.hasOwnership;

        this->material = other.material;
        this->hasOwnership = false;

        return *this;
    }
};

}
