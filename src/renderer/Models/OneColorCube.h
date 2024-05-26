#pragma once

#include "Models/Model.h"

#include "Materials/PlainColorMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class OneColorCube : public Model {
private:
    Surface surface;

public:
    OneColorCube() = delete;

    OneColorCube(
            const Shader& shader,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        SurfaceBuilder builder;
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        Vertex vertices[] = {
            // front
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            // back
            { { -0.5f, -0.5, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            // up
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            // bottom
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            // left
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            // right
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builder.SetMesh(std::make_shared<UnindexedMesh>(vertices, sizeof(vertices), layout));
        builder.SetVertexCount(6 * 6);
        builder.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.8f, 0.2f, 0.3f, 1.0f }));
        this->surface = builder.Build();
    }

    std::vector<Surface> GetSurfaces() const override {
        return std::vector<Surface> { this->surface };
    }
};

}