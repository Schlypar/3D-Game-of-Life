#pragma once

#include <memory>

#include "Model.h"

#include "Materials/PlainColorMaterial.h"
#include "Meshes/UnindexedMesh.h"

namespace GoL {

class SixColorCube : public Model<Vertex> {
private:
    Surface<Vertex> surfaces[6];

public:
    SixColorCube() = delete;

    SixColorCube(
            const Shader& shader,
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& rotation = glm::vec3(0.0f),
            float scaleFactor = 1.0f
    )
        : Model(position, rotation, scaleFactor) {
        SurfaceBuilder<Vertex> builderFront;
        SurfaceBuilder<Vertex> builderBack;
        SurfaceBuilder<Vertex> builderTop;
        SurfaceBuilder<Vertex> builderBottom;
        SurfaceBuilder<Vertex> builderLeft;
        SurfaceBuilder<Vertex> builderRight;

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);

        MaterialLibrary::AddMaterial({ "PlainColorMaterial", new PlainColorMaterial(shader, glm::vec4 { 0.2f, 0.3f, 0.6f, 1.0f }) });

        Vertex front[] = {
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
        };
        builderFront.SetVertexCount(6);
        builderFront.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(front, sizeof(front), layout));
        builderFront.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        Vertex back[] = {
            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
        };
        builderBack.SetVertexCount(6);
        builderBack.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(back, sizeof(back), layout));
        builderBack.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        Vertex up[] = {
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
        };
        builderTop.SetVertexCount(6);
        builderTop.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(up, sizeof(up), layout));
        builderTop.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        Vertex bottom[] = {
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },
        };
        builderBottom.SetVertexCount(6);
        builderBottom.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(bottom, sizeof(bottom), layout));
        builderBottom.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        Vertex left[] = {
            { { -0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },

            { { -0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { -0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },
        };
        builderLeft.SetVertexCount(6);
        builderLeft.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(left, sizeof(left), layout));
        builderLeft.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        Vertex right[] = {
            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },

            { { 0.5f, -0.5f, -0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, 0.5f, 0.5f }, glm::vec3 { 0.0f } },
            { { 0.5f, -0.5f, 0.5f }, glm::vec3 { 0.0f } },
        };
        builderRight.SetVertexCount(6);
        builderRight.SetMesh(std::make_shared<UnindexedMesh<Vertex>>(right, sizeof(right), layout));
        builderRight.SetMaterial(MaterialLibrary::GetMaterial("PlainColorMaterial"));

        this->surfaces[0] = builderFront.Build();
        this->surfaces[1] = builderBack.Build();
        this->surfaces[2] = builderTop.Build();
        this->surfaces[3] = builderBottom.Build();
        this->surfaces[4] = builderLeft.Build();
        this->surfaces[5] = builderRight.Build();
    }

    std::vector<Surface<Vertex>> GetSurfaces() const override {
        return std::vector<Surface<Vertex>>(this->surfaces, this->surfaces + 6);
    }
};
}
