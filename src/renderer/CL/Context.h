#pragma once


#include <CL/cl.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Buffer.h"
#include "CommandQueue.h"
#include "Device.h"
#include "Kernel.h"

namespace GoL {
namespace CL {

    struct Context {

        cl_context ctx;
        std::vector<cl_context_properties> properties;
        std::vector<Device> devices;
        std::vector<cl_device_id> deviceIDs;
        void(CL_CALLBACK* error_callback)(const char*, const void*, std::size_t, void*);
        void* user_data_for_callback;
        std::unordered_map<std::string, Buffer> buffers;
        std::unordered_map<std::string, Kernel*> kernels;

        Context() {
            ctx = nullptr;
        }

        Context(
                std::vector<cl_context_properties> _properties, // specify cl_platform in props
                std::vector<Device> _devices,
                void(CL_CALLBACK* pfn_notify)(const char*, const void*, std::size_t, void*),
                void* _user_data_for_callback
        )
            : properties(_properties)
            , devices(_devices)
            , user_data_for_callback(_user_data_for_callback) {

            this->error_callback = pfn_notify;

            deviceIDs = Device::GetDevicesID(devices);

            cl_int err_ret;
            ctx = clCreateContext(
                    properties.size() ? properties.data() : nullptr,
                    devices.size(),
                    deviceIDs.data(),
                    error_callback,
                    user_data_for_callback,
                    &err_ret
            );
            if (err_ret != CL_SUCCESS) {
                throw(std::runtime_error("CL::Context constructor failed"));
            }

            for (auto& dev : devices) {
                dev.queue = CommandQueue(this->ctx, dev.id, nullptr);
            }
        }

        void Release() {
            if (ctx != nullptr) {
                for (auto& kernel : kernels) {
                    if (kernel.second) {
                        kernel.second->Release();
                        delete kernel.second;
                        kernel.second = nullptr;
                    }
                }

                clReleaseContext(ctx);
                ctx = nullptr;
            }
        }
        
        bool AddBufferSafe(std::string strID, cl_map_flags flags, std::size_t size, void* data) {
            if (!buffers.contains(strID)) {
               buffers[strID] = { ctx, flags, size, data };
            }
        }

        bool AddBufferSafe(std::string strID, cl_map_flags flags, std::size_t size, GLuint vbo) {
            if (!buffers.contains(strID)) {
               buffers[strID] = { ctx, flags, size, vbo };
            }
        }

        cl_command_queue DeviceQueue(uint deviceID) {
            return devices[deviceID].queue.id;
        }
    };

}
}