#pragma once

#include <CL/cl.h>

namespace GoL {
namespace CL {

    struct CommandQueue {
        cl_command_queue id;

        CommandQueue()
            : id(nullptr) {
        }

        CommandQueue(cl_context ctx, cl_device_id dev, const cl_command_queue_properties* props) {
            cl_int errcode_ret;
            id = clCreateCommandQueueWithProperties(ctx, dev, props, &errcode_ret);
            if (errcode_ret != CL_SUCCESS) {
                throw(std::runtime_error("CL::CommandQueue constructor failed"));
            }
        }

        void Release() {
            if (id) {
                clReleaseCommandQueue(id);
                id = nullptr;
            }
        }
    };

}
}