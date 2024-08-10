#pragma once

#if defined(__linux__)
#include <GL/glx.h>
#else
#include <GL/wglext.h>
#endif

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
#pragma pack(push, 1)

    struct clVertex {
        Vertex vertex;
        cl_float matrixIndex;
    };

#pragma pack(pop)

    CLHandler() {
    }

    CLHandler& operator=(CLHandler&& oth) {
        batcherCtx = oth.batcherCtx;
        oth.batcherCtx = CL::Context();

        return *this;
    }

    CLHandler(CL::Platform& platform, CL::Device& device) {
        batcherCtx = CL::Context({

// We need to add information about the OpenGL context with
// which we want to exchange information with the OpenCL context.
#if defined(WIN32)
                                         // We should first check for cl_khr_gl_sharing extension.
                                         CL_GL_CONTEXT_KHR,
                                         (cl_context_properties) wglGetCurrentContext(),
                                         CL_WGL_HDC_KHR,
                                         (cl_context_properties) wglGetCurrentDC(),
#elif defined(__linux__)
                                         // We should first check for cl_khr_gl_sharing extension.
                                         CL_GL_CONTEXT_KHR,
                                         (cl_context_properties) glXGetCurrentContext(),
                                         CL_GLX_DISPLAY_KHR,
                                         (cl_context_properties) glXGetCurrentDisplay(),
#elif defined(__APPLE__)
// We should first check for cl_APPLE_gl_sharing extension.
#if 0
            // This doesn't work.
            CL_GL_CONTEXT_KHR , (cl_context_properties) CGLGetCurrentContext() ,
            CL_CGL_SHAREGROUP_KHR , (cl_context_properties) CGLGetShareGroup( CGLGetCurrentContext() ) ,
#else
                                         CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties) CGLGetShareGroup(CGLGetCurrentContext()),
#endif
#endif

                                         CL_GL_CONTEXT_KHR,
                                         (cl_context_properties) glXGetCurrentContext(),
                                         CL_GLX_DISPLAY_KHR,
                                         (cl_context_properties) glXGetCurrentDisplay(),
                                         CL_CONTEXT_PLATFORM,
                                         (cl_context_properties) platform.id,
                                         0 },
                                 { device },
                                 nullptr,
                                 nullptr);
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

    std::vector<Vertex> CalculateModelMatrices(std::vector<clVertex> vertices, std::vector<glm::mat4> matrices, VertexBuffer& vbo, std::size_t vboSize) {
        auto& kernel = batcherCtx.kernels[batcherKeyCL];

        std::size_t vertexSize = vertices.size();
        std::size_t VboSize = vboSize ? vboSize : vertexSize * sizeof(Vertex);

        // non-zero vboSize here and further means cl_khr_gl_sharing mode
        std::vector<Vertex> vboData(!vboSize ? VboSize : 0);

        if (vboSize) {
            batcherCtx.AddBufferSafe(clvboBufferID, CL_MEM_WRITE_ONLY, VboSize, vbo.GetID());
        } else {
            batcherCtx.AddBufferSafe(clvboBufferID, CL_MEM_WRITE_ONLY, VboSize, nullptr);
        }
        batcherCtx.AddBufferSafe(matrixBufferID, CL_MEM_READ_ONLY, matrices.size() * sizeof(cl_float16), nullptr);
        batcherCtx.AddBufferSafe(vertexBufferID, CL_MEM_READ_ONLY, vertexSize * sizeof(clVertex), nullptr);

        std::size_t singleVertexSizeInFloats = sizeof(clVertex) / sizeof(cl_float);

        if (!this->SetKernelArgs<CL::Buffer, CL::Buffer, CL::Buffer, kernelArg>(
                    batcherKeyCL,
                    batcherCtx.buffers[clvboBufferID],
                    batcherCtx.buffers[matrixBufferID],
                    batcherCtx.buffers[vertexBufferID],
                    { .data = &singleVertexSizeInFloats, .size = sizeof(singleVertexSizeInFloats) }
            )) {
            throw(std::runtime_error("failed to set kernel args"));
        }

        bool res = true;
        res = res && batcherCtx.buffers[matrixBufferID].Write(batcherCtx.DeviceQueue(0), matrices.size() * sizeof(cl_float16), 0, matrices.data());
        res = res && batcherCtx.buffers[vertexBufferID].Write(batcherCtx.DeviceQueue(0), vertexSize * sizeof(clVertex), 0, vertices.data());

        if (!res) {
            throw(std::runtime_error("faied to write data to buffers"));
        }

        if (vboSize) {
            glFinish();
            if (!batcherCtx.buffers[clvboBufferID].AcquireGL(batcherCtx.DeviceQueue(0))) {
                // error
                return {};
            }
        }

        res = this->RunKernel(
                batcherKeyCL,
                { vertices.size() },
                {},
                {}
        );
        if (!res) {
            // error
            return {};
        }

        if (vboSize) {
            batcherCtx.buffers[clvboBufferID].ReleaseGL(batcherCtx.DeviceQueue(0));
        } else {
            if (!batcherCtx.buffers[clvboBufferID].Read(batcherCtx.DeviceQueue(0), VboSize, 0, vboData.data())) {
                throw(std::runtime_error("faied to read data from buffers"));
            }
        }

        this->CLFinish();

        return vboData;
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