#pragma once

#include <vector>
#include <CL/cl.h>
#include <CL/cl_gl.h>

namespace GoL {
namespace CL {

class Buffer {
private:
    bool fromGL;
public:
    cl_mem id;
    cl_mem_flags flags;
    std::size_t size;
    cl_GLuint vbo;

    Buffer() {}

    Buffer(cl_context ctx, cl_mem_flags flags, std::size_t size, void* data) {
        cl_int errcode_ret;
        id = clCreateBuffer(ctx, flags, size, data, &errcode_ret);
        if (errcode_ret != CL_SUCCESS) {
            throw(std::runtime_error("CL::Buffer constructor failed"));
        }

        fromGL = false;
        this->flags = flags;
        this->size = size;
    }

    Buffer(cl_context ctx, cl_mem_flags flags, std::size_t size, cl_GLuint glbuff) {
        cl_int errcode_ret;
        id = clCreateFromGLBuffer(ctx, flags, glbuff, &errcode_ret);
        if (errcode_ret != CL_SUCCESS) {
            throw(std::runtime_error("CL::Buffer constructor from GL failed"));
        }

        fromGL = true;
        this->flags = flags;
        this->size = size;
        this->vbo = glbuff;
    }

    // size of buffer id to set kernel arg
    std::size_t sizeArg() {
        return sizeof(id);
    }

    // ptr to buffer id to set kernel arg
    void* dataArg() {
        return &id;
    }

    bool Read(cl_command_queue queue, std::size_t size, std::size_t offset, void* ptr) {
        return CL_SUCCESS == clEnqueueReadBuffer(queue, id, true, offset, size, ptr, 0, nullptr, nullptr);
    }

    bool Write(cl_command_queue queue, std::size_t size, std::size_t offset, void* ptr) {
        return CL_SUCCESS == clEnqueueWriteBuffer(queue, id, true, offset, size, ptr, 0, nullptr, nullptr);
    }

    bool AcquireGL(cl_command_queue queue) {
        return fromGL && CL_SUCCESS == clEnqueueAcquireGLObjects(queue, 1, &id, 0, nullptr, nullptr);
    }

    bool ReleaseGL(cl_command_queue queue) {
        return fromGL && CL_SUCCESS == clEnqueueReleaseGLObjects(queue, 1, &id, 0, nullptr, nullptr);
    }

    static bool AcquireGLs(cl_command_queue queue, std::vector<Buffer> buffs) {
        int i = 0;
        auto ids = std::vector<cl_mem>(buffs.size());
        for (auto& buff : buffs) {
            if (!buff.fromGL) {
                return false;
            }

            ids[i++] = buff.id;
        }

        return CL_SUCCESS == clEnqueueAcquireGLObjects(queue, ids.size(), ids.data(), 0, nullptr, nullptr);
    }

    static bool ReleaseGLs(cl_command_queue queue, std::vector<Buffer> buffs) {
        int i = 0;
        auto ids = std::vector<cl_mem>(buffs.size());
        for (auto& buff : buffs) {
            if (!buff.fromGL) {
                return false;
            }

            ids[i++] = buff.id;
        }

        return CL_SUCCESS == clEnqueueReleaseGLObjects(queue, ids.size(), ids.data(), 0, nullptr, nullptr);
    }
};


}
}