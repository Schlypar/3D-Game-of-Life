#pragma once

#include <memory>

#include "Model.h"

#include "Materials/PlainColorMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class SixColorCube : public Model {
private:
    struct Vertex {
        glm::vec3 position;
    };

private:
    Surface surfaces[6];

public:
    SixColorCube() = delete;

    SixColorCube(
            const Shader& shader,
            Surface surfaces[6] = nullptr,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        if (surfaces == nullptr) {
            SurfaceBuilder builderFront;
            SurfaceBuilder builderBack;
            SurfaceBuilder builderTop;
            SurfaceBuilder builderBottom;
            SurfaceBuilder builderLeft;
            SurfaceBuilder builderRight;

            VertexBufferLayout layout;
            layout.Push<float>(3);

            Vertex front[] = {
                { { 0.5f, -0.5f, -0.5f } },
                { { -0.5f, -0.5, -0.5f } },
                { { 0.5f, 0.5f, -0.5f } },

                { { 0.5f, 0.5f, -0.5f } },
                { { -0.5f, -0.5, -0.5f } },
                { { -0.5f, 0.5f, -0.5f } },
            };
            builderFront.SetVertexCount(6);
            builderFront.SetMesh(std::make_shared<UnindexedMesh>(front, sizeof(front), layout));
            builderFront.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.8f, 0.2f, 0.3f, 1.0f }));

            Vertex back[] = {
                { { -0.5f, -0.5, 0.5f } },
                { { 0.5f, -0.5f, 0.5f } },
                { { 0.5f, 0.5f, 0.5f } },

                { { -0.5f, -0.5, 0.5f } },
                { { 0.5f, 0.5f, 0.5f } },
                { { -0.5f, 0.5f, 0.5f } },
            };
            builderBack.SetVertexCount(6);
            builderBack.SetMesh(std::make_shared<UnindexedMesh>(back, sizeof(back), layout));
            builderBack.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.2f, 0.8f, 0.3f, 1.0f }));

            Vertex up[] = {
                { { 0.5f, 0.5f, -0.5f } },
                { { -0.5f, 0.5f, -0.5f } },
                { { 0.5f, 0.5f, 0.5f } },

                { { 0.5f, 0.5f, 0.5f } },
                { { -0.5f, 0.5f, -0.5f } },
                { { -0.5f, 0.5f, 0.5f } },
            };
            builderTop.SetVertexCount(6);
            builderTop.SetMesh(std::make_shared<UnindexedMesh>(up, sizeof(up), layout));
            builderTop.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.3f, 0.2f, 0.8f, 1.0f }));

            Vertex bottom[] = {
                { { -0.5f, -0.5f, -0.5f } },
                { { 0.5f, -0.5f, -0.5f } },
                { { 0.5f, -0.5f, 0.5f } },

                { { -0.5f, -0.5f, -0.5f } },
                { { 0.5f, -0.5f, 0.5f } },
                { { -0.5f, -0.5f, 0.5f } },

            };
            builderBottom.SetVertexCount(6);
            builderBottom.SetMesh(std::make_shared<UnindexedMesh>(bottom, sizeof(bottom), layout));
            builderBottom.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.4f, 0.4f, 0.4f, 1.0f }));

            Vertex left[] = {
                { { -0.5f, 0.5f, -0.5f } },
                { { -0.5f, -0.5f, -0.5f } },
                { { -0.5f, 0.5f, 0.5f } },

                { { -0.5f, 0.5f, 0.5f } },
                { { -0.5f, -0.5f, -0.5f } },
                { { -0.5f, -0.5f, 0.5f } },
            };
            builderLeft.SetVertexCount(6);
            builderLeft.SetMesh(std::make_shared<UnindexedMesh>(left, sizeof(left), layout));
            builderLeft.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.6f, 0.0f, 0.6f, 1.0f }));

            Vertex right[] = {
                { { 0.5f, -0.5f, -0.5f } },
                { { 0.5f, 0.5f, -0.5f } },
                { { 0.5f, 0.5f, 0.5f } },

                { { 0.5f, -0.5f, -0.5f } },
                { { 0.5f, 0.5f, 0.5f } },
                { { 0.5f, -0.5f, 0.5f } },
            };
            builderRight.SetVertexCount(6);
            builderRight.SetMesh(std::make_shared<UnindexedMesh>(right, sizeof(right), layout));
            builderRight.SetMaterial(std::make_shared<PlainColorMaterial>(shader, glm::vec4 { 0.0f, 0.6f, 0.6f, 1.0f }));

            Surface frontSurface = builderFront.Build();
            Surface backSurface = builderBack.Build();
            Surface upSurface = builderTop.Build();
            Surface bottomSurface = builderBottom.Build();
            Surface leftSurface = builderLeft.Build();
            Surface rightSurface = builderRight.Build();

            this->surfaces[0] = frontSurface;
            this->surfaces[1] = backSurface;
            this->surfaces[2] = upSurface;
            this->surfaces[3] = bottomSurface;
            this->surfaces[4] = leftSurface;
            this->surfaces[5] = rightSurface;
        } else {
            this->surfaces[0] = surfaces[0];
            this->surfaces[1] = surfaces[1];
            this->surfaces[2] = surfaces[2];
            this->surfaces[3] = surfaces[3];
            this->surfaces[4] = surfaces[4];
            this->surfaces[5] = surfaces[5];
        }
    }

    std::vector<Surface> GetSurfaces() const override {
        return std::vector<Surface>(this->surfaces, this->surfaces + 6);
    }
};
}