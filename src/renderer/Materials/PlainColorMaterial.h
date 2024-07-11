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

    void SetColor(const glm::vec4& color) {
        this->color = color;
    }

    void Bind() override {
        this->BaseBind();
        shader.SetUniform4f("Color", color.r, color.g, color.b, color.a);
    }
};

}
