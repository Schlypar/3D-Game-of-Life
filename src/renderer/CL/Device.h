#pragma once

#include <CL/cl.h>
#include <sstream>
#include <string>
#include <vector>

#include "CommandQueue.h"

namespace GoL {
namespace CL {

    struct Device {
        cl_device_id id;
        std::string name;
        std::string vendor;
        std::vector<std::string> extensions;
        cl_uint maxComputeUnits;
        cl_uint maxWorkItemDimensions;
        cl_uint maxWorkItemSize;
        cl_uint maxWorkGroupSize;

        CommandQueue queue;

        Device() {
        }

        static std::vector<Device> Enumerate(cl_platform_id platformID, cl_uint deviceType) {
            constexpr uint stringLen = 256;
            constexpr cl_uint elementMaxNum = 10;
            constexpr char deviceExtensionSeparator = ' ';

            // cl_device_id deviceIDs[elementMaxNum] = {0};

            cl_uint ret, actualDevicesNum;
            std::size_t nameLen, vendorLen, extensionsLen;

            auto devices = std::vector<Device>(elementMaxNum);

            auto p = platformID;
            auto d = deviceType;

            ret = clGetDeviceIDs(platformID, deviceType, 0, nullptr, &actualDevicesNum);
            if (ret != CL_SUCCESS || actualDevicesNum == 0) {
                return std::vector<Device>();
            }
            std::vector<cl_device_id> deviceIDs(actualDevicesNum);
            ret = clGetDeviceIDs(platformID, deviceType, actualDevicesNum, deviceIDs.data(), nullptr);
            if (ret != CL_SUCCESS || actualDevicesNum == 0) {
                return std::vector<Device>();
            }
            devices.resize(actualDevicesNum);

            auto tmp = std::vector<char>(stringLen);
            for (cl_uint i = 0; i < actualDevicesNum; i++) {
                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_NAME, stringLen, tmp.data(), &nameLen);
                if (ret != CL_SUCCESS) {
                    // handle?
                }
                devices[i].name = std::string(tmp.data());

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_VENDOR, stringLen, tmp.data(), &vendorLen);
                if (ret != CL_SUCCESS) {
                    // handle?
                }
                devices[i].vendor = std::string(tmp.data());

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &devices[i].maxComputeUnits, nullptr);
                if (ret != CL_SUCCESS) {
                    // handle?
                }

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &devices[i].maxWorkItemDimensions, nullptr);
                if (ret != CL_SUCCESS) {
                    // handle?
                }

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(cl_uint), &devices[i].maxWorkItemSize, nullptr);
                if (ret != CL_SUCCESS) {
                    // handle?
                }

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(cl_uint), &devices[i].maxWorkGroupSize, nullptr);
                if (ret != CL_SUCCESS) {
                    // handle?
                }

                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_EXTENSIONS, 0, nullptr, &extensionsLen);
                if (ret != CL_SUCCESS || extensionsLen == 0) {
                    // handle?
                }
                tmp.resize(extensionsLen);
                ret = clGetDeviceInfo(deviceIDs[i], CL_DEVICE_EXTENSIONS, extensionsLen, tmp.data(), nullptr);

                std::stringstream stream(tmp.data());
                std::string entry;
                while (std::getline(stream, entry, deviceExtensionSeparator)) {
                    devices[i].extensions.push_back(std::move(entry));
                }

                devices[i].id = deviceIDs[i];
            }

            return devices;
        }

        static std::vector<cl_device_id> GetDevicesID(std::vector<Device> devices) {
            auto devs = std::vector<cl_device_id>(devices.size());
            for (int i = 0; i < devices.size(); i++) {
                devs[i] = devices[i].id;
            }

            return devs;
        }
    };

}
}