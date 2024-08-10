#pragma once

#include "Material.h"

namespace GoL {

class PlainColorMaterial : public Material {
private:
    glm::vec4 color;

public:
    PlainColorMaterial(const Shader& shader, const glm::vec4& color)
        : Material(shader, "PlainColorMaterial")
        , color(color) {
    }

    PlainColorMaterial(Material* base, const glm::vec4& color)
        : Material(base)
        , color(color) {
    }

    ~PlainColorMaterial() = default;

    bool operator==(Material* other) override {
        PlainColorMaterial* otherMat = dynamic_cast<PlainColorMaterial*>(other);
        if (otherMat == nullptr) {
            return false;
        }

        return this->color == otherMat->color;
    }

    void SetColor(const glm::vec4& color) {
        this->color = color;
    }

    void Bind(glm::mat4* model) override {
        this->BaseBind(model);
        shader.SetUniform4f("Color", color.r, color.g, color.b, color.a);
    }
};

}
