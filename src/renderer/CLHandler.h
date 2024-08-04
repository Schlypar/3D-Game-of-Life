#pragma once

#include <vector>
#include <unordered_map>
#include <CL/cl.h>

#include "CL/Platform.h"
#include "CL/Device.h"
#include "CL/Kernel.h"
#include "CL/Context.h"
#include "CL/Buffer.h"


#include "Models/Model.h"
#include "Models/Vertex.h"
#include "Models/Surface.h"
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

        const std::string batcherKeyCL = "batcher";
        const std::string clvboBufferID = "clvbo";
        const std::string matrixBufferID = "matrix";
        const std::string vertexBufferID = "vertex";
        const std::string vertexPerMatrixID = "vertexpermatrix";

    public:
        CLHandler(CL::Platform& platform, CL::Device& device) {
            batcherCtx = CL::Context({}, {device}, nullptr, nullptr);
            this->AddKernel(batcherKeyCL, KERNEL_CALC_MODEL_MATRICES);
        }

        static std::vector<CL::Platform> GetPlatforms() {
            return CL::Platform::Enumerate();
        }

        static std::vector <CL::Device> GetDevicesForPlatform(CL::Platform& platform) {
            return CL::Device::Enumerate(platform.id, CL_DEVICE_TYPE_GPU);
        }

        bool CalculateModelMatrices(std::vector<Model<Vertex>*> models, std::size_t vertexSize, VertexBuffer& vbo) {
            auto& kernel = batcherCtx.kernels[batcherKeyCL];

            std::size_t vertexCount = 0, i = 0;
            std::size_t vboSize = vertexSize;

            std::vector<cl_float16> modelsBuffer(models.size());
            std::vector<cl_float> vertexBuffer(vertexSize * sizeof(Vertex) / sizeof(float));
            std::vector<cl_uint> verticesForMatricesBorders(models.size() + 1);

            std::memset(verticesForMatricesBorders.data(), 0, verticesForMatricesBorders.size() * sizeof(cl_uint));
            auto modelsPtr = modelsBuffer.data();
            auto vertexPtr = vertexBuffer.data();
            for (auto& m : models) {
                auto& modelMatrix = m->GetModelMatrix();
                std::memcpy(modelsPtr, &modelMatrix, sizeof(glm::mat4));

                verticesForMatricesBorders[i++] = vertexCount;

                for (auto& surface : m->GetSurfaces()) {
                    auto& data = surface.mesh->GetData();
                    std::memcpy(vertexPtr, data.bytes, data.size);
                    vertexPtr += data.size;

                    vertexCount += surface.vertexCount;
                }
            }
            verticesForMatricesBorders[i] = vertexCount;

            if (batcherCtx.devices[0].maxComputeUnits < vertexCount * vertexCount * vertexCount) {
                return false;
            }

            if (!batcherCtx.buffers.contains(clvboBufferID)) {
                batcherCtx.buffers[clvboBufferID] = {batcherCtx.ctx, (cl_map_flags)CL_MEM_WRITE_ONLY, vboSize, (cl_GLuint)vbo.GetID()};
            }

            if (!batcherCtx.buffers.contains(matrixBufferID)) {
                batcherCtx.buffers[matrixBufferID] = {batcherCtx.ctx, (cl_map_flags)CL_MEM_READ_ONLY, models.size() * sizeof(cl_float16), nullptr};
            }

            if (!batcherCtx.buffers.contains(vertexBufferID)) {
                batcherCtx.buffers[vertexBufferID] = {batcherCtx.ctx, (cl_map_flags)CL_MEM_READ_ONLY, vertexSize * sizeof(Vertex), nullptr};
            }

            cl_uint singleVertexSize = (cl_uint)sizeof(Vertex);

            this->SetKernelArgs<CL::Buffer, CL::Buffer, CL::Buffer, CL::Buffer, kernelArg>(
                batcherKeyCL,
                batcherCtx.buffers[clvboBufferID],
                batcherCtx.buffers[vertexPerMatrixID],
                batcherCtx.buffers[matrixBufferID],
                batcherCtx.buffers[vertexBufferID],
                {.data = &singleVertexSize, .size = sizeof(singleVertexSize)}
            );

            batcherCtx.buffers[vertexPerMatrixID].Write(batcherCtx.devices[0].queue.id, verticesForMatricesBorders.size() * sizeof(cl_uint), 0, verticesForMatricesBorders.data());
            batcherCtx.buffers[matrixBufferID].Write(batcherCtx.devices[0].queue.id, modelsBuffer.size() * sizeof(cl_float16), 0, modelsBuffer.data());
            batcherCtx.buffers[vertexBufferID].Write(batcherCtx.devices[0].queue.id, vertexBuffer.size() * sizeof(cl_float), 0, vertexBuffer.data());
            
            return this->RunKernel(
                batcherKeyCL,
                {models.size()},
                {},
                {}
            );
        }

    // private: ?
        bool AddKernel(std::string kernelName, const unsigned char* source) {
            if (!batcherCtx.kernels.contains(kernelName)) {
                batcherCtx.kernels[kernelName].Release();
            }

            auto kernel = CL::Kernel(kernelName, {source}, batcherCtx.ctx, (cl_uint)batcherCtx.devices.size(), (cl_device_id*)batcherCtx.devices.data());
            batcherCtx.kernels[kernelName] = std::move(kernel);
            return true;
        }
        template <CL::KernelArg Arg, CL::KernelArg ...Args>
        bool SetKernelArgs(
            std::string kernelName,
            Arg arg, Args... args) {

            if (!batcherCtx.kernels.contains(kernelName)) {
                return false;
            }

            auto& kernel = batcherCtx.kernels[kernelName];
            return kernel.SetArg(0, arg, args...);
        }

        bool RunKernel(
            std::string kernelName,
            std::vector<std::size_t> dimensionSizes,
            std::vector<std::size_t> workgroupSizes,
            std::vector<std::size_t> workOffsets) {

            if (!batcherCtx.kernels.contains(kernelName)) {
                return false;
            }
            auto& kernel = batcherCtx.kernels[kernelName];
            
            return CL_SUCCESS == kernel.Run(batcherCtx.devices[0].queue.id, dimensionSizes, workOffsets, workgroupSizes);
        }

        void CLFinish() {
            clFinish(batcherCtx.devices[0].queue.id);
        }

        ~CLHandler() {
            batcherCtx.Release();
        }
    };
    

}