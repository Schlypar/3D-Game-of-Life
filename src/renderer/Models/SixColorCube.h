#pragma once

#include <memory>

#include "Model.h"

#include "Materials/PlainColorMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class SixColorCube : public Model {
private:
    Surface surfaces[6];

public:
    SixColorCube() = delete;

    SixColorCube(
            const Shader& shader,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        SurfaceBuilder builderFront;
        SurfaceBuilder builderBack;
        SurfaceBuilder builderTop;
        SurfaceBuilder builderBottom;
        SurfaceBuilder builderLeft;
        SurfaceBuilder builderRight;

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        Vertex front[] = {
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
        };
        builderFront.SetVertexCount(6);
        builderFront.SetMesh(std::make_shared<UnindexedMesh>(front, sizeof(front), layout));
        builderFront.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.8f, 0.2f, 0.3f, 1.0f }));

        Vertex back[] = {
            { { -0.5f, -0.5, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builderBack.SetVertexCount(6);
        builderBack.SetMesh(std::make_shared<UnindexedMesh>(back, sizeof(back), layout));
        builderBack.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.2f, 0.8f, 0.3f, 1.0f }));

        Vertex up[] = {
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builderTop.SetVertexCount(6);
        builderTop.SetMesh(std::make_shared<UnindexedMesh>(up, sizeof(up), layout));
        builderTop.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.3f, 0.2f, 0.8f, 1.0f }));

        Vertex bottom[] = {
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builderBottom.SetVertexCount(6);
        builderBottom.SetMesh(std::make_shared<UnindexedMesh>(bottom, sizeof(bottom), layout));
        builderBottom.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.4f, 0.4f, 0.4f, 1.0f }));

        Vertex left[] = {
            { { -0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { -0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builderLeft.SetVertexCount(6);
        builderLeft.SetMesh(std::make_shared<UnindexedMesh>(left, sizeof(left), layout));
        builderLeft.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.6f, 0.0f, 0.6f, 1.0f }));

        Vertex right[] = {
            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },

            { { 0.5f, -0.5f, -0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec4 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec4 { 0.0f } },
        };
        builderRight.SetVertexCount(6);
        builderRight.SetMesh(std::make_shared<UnindexedMesh>(right, sizeof(right), layout));
        builderRight.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.0f, 0.6f, 0.6f, 1.0f }));

        this->surfaces[0] = builderFront.Build();
        this->surfaces[1] = builderBack.Build();
        this->surfaces[2] = builderTop.Build();
        this->surfaces[3] = builderBottom.Build();
        this->surfaces[4] = builderLeft.Build();
        this->surfaces[5] = builderRight.Build();
    }

    std::vector<Surface> GetSurfaces() const override {
        return std::vector<Surface>(this->surfaces, this->surfaces + 6);
    }
};
}