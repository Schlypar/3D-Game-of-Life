#pragma once

#include "Models/Model.h"

#include "Materials/PlainColorMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class CubeFrame : public Model<Vertex> {
private:
    Surface<Vertex> surface;

public:
    CubeFrame() = delete;

    CubeFrame(
            const Shader& shader,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            const glm::vec4& color = glm::vec4 { 0.2f, 0.3f, 0.6f, 1.0f },
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        SurfaceBuilder<Vertex> builder;
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        // MaterialLibrary::AddMaterial({ "PlainColorMaterial", new PlainColorMaterial(shader, glm::vec4 { 0.2f, 0.3f, 0.6f, 1.0f }) });
        auto material = new PlainColorMaterial(shader, color);

        Vertex vertices[] = {
            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5, -0.5f }, glm::vec3 { 0.0f } },



            { { 0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5, -0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5, -0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, 0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5, -0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
        };
        builder.SetMesh(new UnindexedMesh<Vertex>(vertices, sizeof(vertices), layout));
        builder.SetVertexCount(sizeof(vertices) / sizeof(Vertex));
        // builder.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));
        builder.SetMaterial(material);
        builder.SetMode(GL_LINES);
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
