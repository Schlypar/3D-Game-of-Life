#pragma once

#include "PlainColorMaterial.h"

namespace GoL {

class VoxelMaterial : public Material {
public:
    VoxelMaterial(const Shader& shader)
        : Material(shader, "VoxelShader") {
    }

    VoxelMaterial(Material* base)
        : Material(base) {
    }

    ~VoxelMaterial() = default;

    bool operator==(Material* other) override {
        VoxelMaterial* otherMat = dynamic_cast<VoxelMaterial*>(other);
        if (otherMat == nullptr) {
            return false;
        }
    }

    void Bind() override {
        this->shader.Bind();
        this->shader.SetUniformMat4f("ProjectionView", *this->projectionView);
    }
};

}
