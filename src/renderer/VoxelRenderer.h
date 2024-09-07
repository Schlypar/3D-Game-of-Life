#pragma once

#include "precompiled.h"

#include "Camera.h"

#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Models/CubeVoxel.h"
#include "Materials/Material.h"
#include "Materials/VoxelMaterial.h"

namespace GoL {

class VoxelRenderer {
private:
    std::vector<Vertex> modelVertices;
    std::vector<glm::mat4> voxelMatrices;
    MaterialPointer material;
    GLenum drawMode;
    bool firstSubmit = true;
    bool changed = true;

    VertexArray vertexArray;
    VertexBuffer vertexBuffer;
    VertexBuffer matrixBuffer;
    unsigned int vertexSize = 0;
    unsigned int matrixSize = 0;

public:
    VoxelRenderer()
        : vertexArray()
        , vertexBuffer(nullptr, 0, GL_DYNAMIC_DRAW) 
        , matrixBuffer(nullptr, 0, GL_DYNAMIC_DRAW) {

        this->vertexArray.Unbind();
        this->vertexBuffer.Unbind();
        this->matrixBuffer.Unbind();
    }

    void Submit(CubeVoxel* model) {
        this->voxelMatrices.push_back(model->GetModelMatrix());

        if (this->firstSubmit) {
            this->setupAttributes();
            
            this->vertexSize = model->GetSurfaces().front().mesh->GetData().size;

            this->vertexBuffer.Bind();
            this->vertexBuffer.Realloc(this->vertexSize, GL_STATIC_DRAW);
            this->vertexBuffer.Write(model->GetSurfaces().front().mesh->GetData().bytes, this->vertexSize);
            this->vertexBuffer.Unbind();

            this->material = model->GetMaterial();

            this->drawMode = model->GetSurfaces().front().mode;

            this->firstSubmit = false;
        }


        this->changed = true;
    }

    void Reset() {
        this->voxelMatrices.clear();
        this->changed = true;
    }

    void DrawSubmitted(const Camera& camera) {
        if (this->changed) {
            this->loadBuffers();
            this->changed = false;
        }

        glm::mat4 projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
        
        this->vertexArray.Bind();
        drawSurfaces(projectionView);
        this->vertexArray.Unbind();
    }

private:
    void setupAttributes() {
        const uint vertex_location = 0, matrices_location = 2;

        this->vertexArray.Bind();

        // vertices layout
        this->vertexBuffer.Bind();

        // coords
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, (GLsizei) 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(vertex_location);
        // colors
        glVertexAttribPointer(vertex_location + 1, 3, GL_FLOAT, GL_FALSE, (GLsizei) 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(vertex_location + 1);

        // matrices layout
        this->matrixBuffer.Bind();

        for (unsigned int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(matrices_location + i);
            glVertexAttribPointer(matrices_location + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (const GLvoid*) (sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(matrices_location + i, 1);
        }

        this->vertexArray.Unbind();
        this->matrixBuffer.Unbind();
    }

    void loadBuffers() {
        if (this->voxelMatrices.empty()) {
            return;
        }

        this->matrixBuffer.Bind();

        auto bufferSize = sizeof(glm::mat4) * this->voxelMatrices.size();
        if (bufferSize > matrixSize) {
            this->matrixBuffer.Realloc(bufferSize, GL_DYNAMIC_DRAW);
            matrixSize = bufferSize;
        }

        this->matrixBuffer.Write(this->voxelMatrices.data(), bufferSize);
        this->matrixBuffer.Unbind();

    }

    void drawSurfaces(const glm::mat4& projectionView) {
        if (this->voxelMatrices.empty()) {
            return;
        }

        this->material.material->SetProjectionView(projectionView);
        this->material.material->Bind();

        glDrawArraysInstanced(this->drawMode, 0, this->vertexSize / sizeof(Vertex), this->voxelMatrices.size());
        auto errorCode = glGetError();
        if (errorCode != GL_NO_ERROR) {
            auto errorString = glad_glGetError();
            OPENGL_ERROR("DrawArraysInstanced {}: {}", errorCode, errorString);
        }
    }
};



}
