#pragma once

#include <GL/glx.h>

#include <CL/cl.h>
#include <unordered_map>
#include <vector>

#include "CL/Buffer.h"
#include "CL/Context.h"
#include "CL/Device.h"
#include "CL/Kernel.h"
#include "CL/Platform.h"

#include "Models/Model.h"
#include "Models/Surface.h"
#include "Models/Vertex.h"
#include "VertexBuffer.h"

#include "kernel_calc_model_matrices.h"

namespace GoL {

class CLHandler {
private:
    struct kernelArg {
        void* data;
        std::size_t size;

        inline void* dataArg() {
            return data;
        }

        inline std::size_t sizeArg() {
            return size;
        }
    };

    CL::Context batcherCtx;

    std::string batcherKeyCL = "batcher";
    std::string clvboBufferID = "clvbo";
    std::string matrixBufferID = "matrix";
    std::string vertexBufferID = "vertex";
    std::string vertexPerMatrixID = "vertexpermatrix";

public:
    CLHandler() {
    }

    CLHandler& operator=(CLHandler&& oth) {
        batcherCtx = oth.batcherCtx;
        oth.batcherCtx = CL::Context();

        return *this;
    }

    CLHandler(CL::Platform& platform, CL::Device& device) {
        batcherCtx = CL::Context({
            CL_GL_CONTEXT_KHR,
            (cl_context_properties)glXGetCurrentContext(),
            // CL_GLX_DISPLAY_KHR,
            // (cl_context_properties)glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM,
            (cl_context_properties)platform.id,
            0
        }, { device }, nullptr, nullptr);
        std::vector<char> src(sizeof(KERNEL_CALC_MODEL_MATRICES));
        std::memcpy(src.data(), KERNEL_CALC_MODEL_MATRICES, src.size());
        this->AddKernel(src);
    }

    static std::vector<CL::Platform> GetPlatforms() {
        return CL::Platform::Enumerate();
    }

    static std::vector<CL::Device> GetDevicesForPlatform(CL::Platform& platform) {
        return CL::Device::Enumerate(platform.id, CL_DEVICE_TYPE_GPU);
    }

    bool CalculateModelMatrices(std::vector<Model<Vertex>*> models, std::size_t vertexSize, VertexBuffer& vbo) {
        auto& kernel = batcherCtx.kernels[batcherKeyCL];

        std::size_t vertexCount = 0, i = 0;
        std::size_t vboSize = vertexSize * sizeof(Vertex);

        std::vector<cl_float16> modelsBuffer(models.size());
        std::vector<cl_float> vertexBuffer(vertexSize * sizeof(Vertex) / sizeof(cl_float));
        std::vector<cl_uint> verticesForMatricesBorders(models.size() + 1);

        std::memset(verticesForMatricesBorders.data(), 0, verticesForMatricesBorders.size() * sizeof(cl_uint));
        auto modelsPtr = modelsBuffer.data();
        auto vertexPtr = vertexBuffer.data();
        for (auto& m : models) {
            auto& modelMatrix = m->GetModelMatrix();
            float* ptr = (float*) modelsPtr;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    ptr[i * 4 + j] = modelMatrix[i][j];
                }
            }
            modelsPtr++;

            verticesForMatricesBorders[i++] = vertexCount;

            for (auto& surface : m->GetSurfaces()) {
                auto& data = surface.mesh->GetData();
                std::memcpy(vertexPtr + vertexCount * sizeof(Vertex) / sizeof(cl_float), data.bytes, data.size);

                vertexCount += surface.vertexCount;
            }
        }
        verticesForMatricesBorders[i] = vertexCount;

        batcherCtx.AddBufferSafe(vertexPerMatrixID, CL_MEM_READ_ONLY, verticesForMatricesBorders.size() * sizeof(cl_uint), nullptr);
        batcherCtx.AddBufferSafe(clvboBufferID, CL_MEM_WRITE_ONLY, vboSize, (cl_GLuint) vbo.GetID());
        batcherCtx.AddBufferSafe(matrixBufferID, CL_MEM_READ_ONLY, models.size() * sizeof(cl_float16), nullptr);
        batcherCtx.AddBufferSafe(vertexBufferID, CL_MEM_READ_ONLY, vertexSize * sizeof(Vertex), nullptr);

        cl_uint singleVertexSizeInFloats = (cl_uint) (sizeof(Vertex) / sizeof(cl_float));

        glFinish();
        batcherCtx.buffers[clvboBufferID].AcquireGL(batcherCtx.DeviceQueue(0));

        if (!this->SetKernelArgs<CL::Buffer, CL::Buffer, CL::Buffer, CL::Buffer, kernelArg>(
                    batcherKeyCL,
                    batcherCtx.buffers[clvboBufferID],
                    batcherCtx.buffers[vertexPerMatrixID],
                    batcherCtx.buffers[matrixBufferID],
                    batcherCtx.buffers[vertexBufferID],
                    { .data = &singleVertexSizeInFloats, .size = sizeof(singleVertexSizeInFloats) }
            )) {
            throw(std::runtime_error("failed to set kernel args"));
        }

        bool res = true;
        res = res && batcherCtx.buffers[vertexPerMatrixID].Write(batcherCtx.DeviceQueue(0), verticesForMatricesBorders.size() * sizeof(cl_uint), 0, verticesForMatricesBorders.data());
        res = res && batcherCtx.buffers[matrixBufferID].Write(batcherCtx.DeviceQueue(0), modelsBuffer.size() * sizeof(cl_float16), 0, modelsBuffer.data());
        res = res && batcherCtx.buffers[vertexBufferID].Write(batcherCtx.DeviceQueue(0), vertexBuffer.size() * sizeof(cl_float), 0, vertexBuffer.data());

        if (!res) {
            throw(std::runtime_error("faied to write data to buffers"));
        }

        for (std::size_t offset = 0; offset < models.size(); offset += batcherCtx.devices[0].maxComputeUnits) {
            res = this->RunKernel(
                    batcherKeyCL,
                    { std::min((std::size_t) batcherCtx.devices[0].maxComputeUnits, std::min(models.size(), models.size() - offset)) },
                    {},
                    { offset }
            );

            if (!res) {
                batcherCtx.buffers[clvboBufferID].ReleaseGL(batcherCtx.DeviceQueue(0));
                this->CLFinish();
                return res;
            }
        }

        batcherCtx.buffers[clvboBufferID].ReleaseGL(batcherCtx.DeviceQueue(0));
        // if (!batcherCtx.buffers[clvboBufferID].Read(batcherCtx.DeviceQueue(0), vboSize, 0, vbo)) {
        //     throw(std::runtime_error("faied to read data from buffers"));
        // }
        this->CLFinish();

        return res;
    }

    // private: ?
    bool AddKernel(std::vector<char> src) {
        if (batcherCtx.kernels.contains(batcherKeyCL)) {
            batcherCtx.kernels[batcherKeyCL]->Release();
        }

        auto kernel = new CL::Kernel(src.data(), src.size(), batcherCtx.ctx, (cl_uint) batcherCtx.devices.size(), (cl_device_id*) batcherCtx.devices.data());
        batcherCtx.kernels[batcherKeyCL] = kernel;
        return true;
    }

    template <CL::KernelArg Arg, CL::KernelArg... Args>
    bool SetKernelArgs(
            std::string kernelName,
            Arg arg,
            Args... args
    ) {

        if (!batcherCtx.kernels.contains(kernelName)) {
            return false;
        }

        auto& kernel = batcherCtx.kernels[kernelName];
        return kernel->SetArg(0, arg, args...);
    }

    bool RunKernel(
            std::string kernelName,
            std::vector<std::size_t> dimensionSizes,
            std::vector<std::size_t> workgroupSizes,
            std::vector<std::size_t> workOffsets
    ) {

        if (!batcherCtx.kernels.contains(kernelName)) {
            return false;
        }
        auto& kernel = batcherCtx.kernels[kernelName];

        return CL_SUCCESS == kernel->Run(batcherCtx.DeviceQueue(0), dimensionSizes, workOffsets, workgroupSizes);
    }

    void CLFinish() {
        if (CL_SUCCESS != clFinish(batcherCtx.DeviceQueue(0))) {
            throw std::runtime_error("failed to finish cl queue");
        }
    }

    ~CLHandler() {
        batcherCtx.Release();
    }
};

}