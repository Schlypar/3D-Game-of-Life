#pragma once

#include <CL/cl.h>
#include <concepts>
#include <string>

namespace GoL {
namespace CL {

    template <typename T>
    concept KernelArg = requires(T x) {
        { x.dataArg() } -> std::convertible_to<void*>;
        { x.sizeArg() } -> std::convertible_to<std::size_t>;
    };

    class Kernel {
    private:
        cl_program program;
        cl_kernel kernel;

    public:
        std::string name;

        Kernel() {
            program = nullptr;
            kernel = nullptr;
        }

        Kernel(Kernel& oth) {
            this->Release();
            program = oth.program;
            kernel = oth.kernel;
        }

        Kernel(Kernel&& oth) {
            program = oth.program;
            kernel = oth.kernel;
            oth.program = nullptr;
            oth.kernel = nullptr;
        }

        Kernel& operator=(Kernel& oth) {
            this->Release();
            program = oth.program;
            kernel = oth.kernel;

            return *this;
        }

        Kernel& operator=(Kernel&& oth) {
            this->Release();
            program = oth.program;
            kernel = oth.kernel;
            oth.program = nullptr;
            oth.kernel = nullptr;

            return *this;
        }

        Kernel(std::string kernelName, std::vector<const unsigned char*> sources, cl_context ctx, cl_uint num_devs, cl_device_id* devs) {
            auto sourceSizes = std::vector<std::size_t>(sources.size());
            for (int i = 0; i < sourceSizes.size(); i++) {
                sourceSizes[i] = sizeof(sources[i]);
            }

            cl_int errcode_ret;
            program = clCreateProgramWithBinary(
                    ctx,
                    num_devs,
                    devs,
                    sourceSizes.data(),
                    sources.data(),
                    nullptr,
                    &errcode_ret
            );
            if (errcode_ret != CL_SUCCESS) {
                // handle
                return;
            }

            kernel = clCreateKernel(program, kernelName.c_str(), &errcode_ret);
            if (errcode_ret != CL_SUCCESS) {
                // handle
            }

            name = kernelName;
        }

        template <KernelArg T>
        bool SetArg(cl_uint arg_index, T arg) {
            return CL_SUCCESS == clSetKernelArg(kernel, arg_index, arg.sizeArg(), arg.dataArg());
        }

        template <KernelArg T, KernelArg... E>
        bool SetArg(cl_uint arg_index, T arg, E... args) {
            return CL_SUCCESS == clSetKernelArg(kernel, arg_index, arg.sizeArg(), arg.dataArg())
                || this->SetArg(arg_index + 1, args...);
        }

        cl_int Run(cl_command_queue queue, std::vector<std::size_t> dimensionSizes, std::vector<std::size_t> workOffsets, std::vector<std::size_t> workgroupSizes) {
            cl_uint dim = dimensionSizes.size();
            auto global_work_offset = workOffsets.size() == dim ? workOffsets.data() : nullptr;
            auto work_group_sizes = workgroupSizes.size() == dim ? workgroupSizes.data() : nullptr;

            return clEnqueueNDRangeKernel(
                    queue,
                    kernel,
                    dim,
                    global_work_offset,
                    dimensionSizes.data(),
                    work_group_sizes,
                    0,
                    nullptr,
                    nullptr
            );
        }

        void Release() {
            if (program && kernel) {
                clReleaseKernel(kernel);
                clReleaseProgram(program);
            }
        }
    };

}
}