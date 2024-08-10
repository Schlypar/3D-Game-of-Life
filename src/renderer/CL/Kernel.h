#pragma once

#include <CL/cl.h>
#include <concepts>
#include <string>
#include <vector>

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
        std::string kernelName;

        Kernel() {
            program = nullptr;
            kernel = nullptr;
        }

        Kernel(Kernel& oth) {
            program = oth.program;
            kernel = oth.kernel;
            kernelName = oth.kernelName;
        }

        Kernel(Kernel&& oth) {
            program = oth.program;
            kernel = oth.kernel;
            kernelName = oth.kernelName;
            oth.program = nullptr;
            oth.kernel = nullptr;
            oth.kernelName = "";
        }

        Kernel& operator=(Kernel& oth) {
            this->Release();
            program = oth.program;
            kernel = oth.kernel;
            kernelName = oth.kernelName;

            return *this;
        }

        Kernel& operator=(Kernel&& oth) {
            this->Release();
            program = oth.program;
            kernel = oth.kernel;
            kernelName = oth.kernelName;
            oth.program = nullptr;
            oth.kernel = nullptr;
            oth.kernelName = "";

            return *this;
        }

        Kernel(const char* source, std::size_t srcSize, cl_context ctx, cl_uint num_devs, cl_device_id* devs) {
            cl_int errcode_ret;
            program = clCreateProgramWithSource(
                    ctx,
                    num_devs,
                    &source,
                    &srcSize,
                    &errcode_ret
            );
            if (errcode_ret != CL_SUCCESS) {
                std::vector<char> msg(100);
                std::sprintf(msg.data(), "CL::Kernel constructor failed: failed to create program (errcode:%d)", errcode_ret);
                throw(std::runtime_error(msg.data()));
            }

            errcode_ret = clBuildProgram(program, num_devs, devs, nullptr, nullptr, nullptr);
            if (errcode_ret != CL_SUCCESS) {
                std::vector<char> msg(100);
                std::sprintf(msg.data(), "CL::Kernel constructor failed: failed to build (compile and link) program (errcode:%d)", errcode_ret);
                throw(std::runtime_error(msg.data()));
            }

            kernelName = GetKernelNameFromSource(source);

            kernel = clCreateKernel(program, kernelName.c_str(), &errcode_ret);
            if (errcode_ret != CL_SUCCESS) {
                std::vector<char> msg(100);
                std::sprintf(msg.data(), "CL::Kernel constructor failed: failed to create kernel (errcode:%d)", errcode_ret);
                throw(std::runtime_error(msg.data()));
            }
        }

        template <KernelArg T>
        bool SetArg(cl_uint arg_index, T arg) {
            return CL_SUCCESS == clSetKernelArg(kernel, arg_index, arg.sizeArg(), arg.dataArg());
        }

        template <KernelArg T, KernelArg... E>
        bool SetArg(cl_uint arg_index, T arg, E... args) {
            return CL_SUCCESS == clSetKernelArg(kernel, arg_index, arg.sizeArg(), arg.dataArg())
                && this->SetArg(arg_index + 1, args...);
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
                program = nullptr;
                kernel = nullptr;
            }
        }

        std::string GetKernelNameFromSource(const char* src) {
            auto s = std::string(src);
            auto start = s.find("__kernel void ");
            s = s.substr(start + sizeof("__kernel void ") - 1);
            auto end = s.find('(');
            return s.substr(0, end);
        }
    };

}
}