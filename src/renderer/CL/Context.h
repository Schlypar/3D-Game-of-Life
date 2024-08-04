#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <CL/cl.h>

#include "Device.h"
#include "Buffer.h"
#include "Kernel.h"
#include "CommandQueue.h"

namespace GoL {
namespace CL {

struct Context {

    cl_context ctx;
    std::vector<cl_context_properties> properties;
    std::vector<Device> devices;
    std::vector<cl_device_id> deviceIDs;
    void (CL_CALLBACK* error_callback)(const char*, const void*, std::size_t, void*);
    void* user_data_for_callback;
    std::unordered_map<std::string, Buffer> buffers;
    std::unordered_map<std::string, Kernel> kernels;

    Context() {
        ctx = nullptr;
    }

    Context(
        std::vector<cl_context_properties> _properties, // specify cl_platform in props
        std::vector<Device> _devices,
        void (CL_CALLBACK* pfn_notify)(const char*, const void*, std::size_t, void*),
        void* _user_data_for_callback)
        : properties(_properties)
        , devices(_devices)
        , user_data_for_callback(_user_data_for_callback) {
    
        this->error_callback = pfn_notify;

        deviceIDs = Device::GetDevicesID(devices);

        cl_int err_ret;
        ctx = clCreateContext(
            properties.size()? properties.data() : nullptr,
            devices.size(), deviceIDs.data(), error_callback, user_data_for_callback, &err_ret);
        if (err_ret != CL_SUCCESS) {
            // handle
        }

        for (auto& dev : devices) {
            dev.queue = CommandQueue(this->ctx, dev.id, 0);
        }
    }

    void Release() {
        if (ctx != nullptr) {
            for (auto& kernel : kernels) {
                kernel.second.Release();
            }

            clReleaseContext(ctx);
        }
    }
};

}
}