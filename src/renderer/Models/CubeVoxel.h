#pragma once

#include "Models/Model.h"

#include "Materials/VoxelMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class CubeVoxel : public Model<Vertex> {
private:
    Surface<Vertex> surface;

public:
    CubeVoxel() = delete;

    CubeVoxel(
            const Shader& shader,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        SurfaceBuilder<Vertex> builder;

        VertexBufferLayout layout;

        this->materialPointer.material = new VoxelMaterial(shader),
        this->materialPointer.hasOwnership = true;

        Vertex vertices[] = {
            // front
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            // back
            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            // up
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            // bottom
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            // left
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            // right
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },

            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.2f, 0.3f, 0.6f } },
        };
        builder.SetMesh(new UnindexedMesh<Vertex>(vertices, sizeof(vertices), layout));
        builder.SetVertexCount(6 * 6);
        builder.SetMaterial(this->materialPointer.material);
        builder.SetMode(GL_TRIANGLES);
        this->surface = builder.Build();
    }

    void SetMaterial(const MaterialPointer& material) override {
        this->materialPointer = material;
        this->surface.material = material.material;
    }

    std::vector<Surface<Vertex>> GetSurfaces() const override {
        return std::vector<Surface<Vertex>> { this->surface };
    }
};

}