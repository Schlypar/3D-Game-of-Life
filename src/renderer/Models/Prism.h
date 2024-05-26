#pragma once

#include "Materials/Material.h"
#include "Materials/PlainColorMaterial.h"
#include "Meshes/IndexedMesh.h"
#include "Model.h"

namespace GoL {

class IndexedPrism : public Model {
private:
    Surface surface;

public:
    IndexedPrism() = delete;

    IndexedPrism(
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
            // up
            { { 0.5f, 0.5f, 0.5f }, { 0.8f, 0.2f, 0.3f } },

            // bottom
            { { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f } },
            { { 0.5f, -0.5f, 0.5f }, { 0.9f, 0.0f, 0.3f } },
            { { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.7f, 0.7f } },
        };

        unsigned int indices[] = {
            1, 0, 2,
            2, 0, 3,
            3, 0, 4,
            4, 0, 1,
            1, 2, 3,
            3, 4, 1,
        };

        builder.SetVertexCount(5);
        builder.SetMesh(std::make_shared<IndexedMesh>(vertices, sizeof(vertices), indices, sizeof(indices) / sizeof(unsigned int), layout));
        builder.SetMaterial(std::make_shared<Material>(shader, "prism"));

        this->surface = builder.Build();
    }

    std::vector<Surface> GetSurfaces() const override {
        return std::vector<Surface> { this->surface };
    }
};

}