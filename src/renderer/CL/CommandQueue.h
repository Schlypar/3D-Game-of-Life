#pragma once

#include <CL/cl.h>


namespace GoL {
namespace CL {

struct CommandQueue {
    cl_command_queue id;

    CommandQueue() {}

    CommandQueue(cl_context ctx, cl_device_id dev, cl_command_queue_properties props) {
        cl_int errcode_ret;
        id = clCreateCommandQueue(ctx, dev, props, &errcode_ret);
        if (errcode_ret != CL_SUCCESS) {
            // handle
        }
    }

    void Release() {
        clReleaseCommandQueue(id);
    }
};


}
}