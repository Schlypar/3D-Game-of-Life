#pragma once

#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <CL/cl.h>

namespace GoL {
namespace CL {

struct Platform {
    cl_platform_id id;
    std::string name;
    std::string vendor;
    std::vector<std::string> extensions;

    Platform() {}

    static std::vector<Platform> Enumerate() {
        constexpr uint stringLen = 256;
        constexpr cl_uint elementMaxNum = 10;
        constexpr char platformExtensionSeparator = ' ';

        cl_platform_id platformsIDs[elementMaxNum] = {0};

        cl_uint ret, actualPlatformsNum;
        std::size_t nameLen, vendorLen, extensionsLen;
        auto platforms = std::vector<Platform>(elementMaxNum);

        ret = clGetPlatformIDs(elementMaxNum, platformsIDs, &actualPlatformsNum);
        if (ret != CL_SUCCESS || actualPlatformsNum == 0) {
            return std::vector<Platform>();
        }
        platforms.resize(actualPlatformsNum);

        auto tmp = std::vector<char>(stringLen);
        for (cl_uint i = 0; i < actualPlatformsNum; i++) {
            ret = clGetPlatformInfo(platformsIDs[i], CL_PLATFORM_NAME, stringLen, tmp.data(), &nameLen);
            if (ret != CL_SUCCESS) {
                continue;
            }
            platforms[i].name = std::string(tmp.data());

            ret = clGetPlatformInfo(platformsIDs[i], CL_PLATFORM_VENDOR, stringLen, tmp.data(), &vendorLen);
            if (ret != CL_SUCCESS) {
                continue;
            }
            platforms[i].vendor = std::string(tmp.data());

            ret = clGetPlatformInfo(platformsIDs[i], CL_PLATFORM_EXTENSIONS, 0, nullptr, &extensionsLen);
            if (ret != CL_SUCCESS || extensionsLen == 0) {
                continue;
            }
            tmp.resize(extensionsLen);
            ret = clGetPlatformInfo(platformsIDs[i], CL_PLATFORM_EXTENSIONS, extensionsLen, tmp.data(), nullptr);
            if (ret != CL_SUCCESS || extensionsLen == 0) {
                continue;
            }

            std::stringstream stream(tmp.data());
            std::string entry;
            while (std::getline(stream, entry, platformExtensionSeparator)) {
                platforms[i].extensions.push_back(std::move(entry));
            }

            platforms[i].id = platformsIDs[i];
        }

        return platforms;
    }
};

}
}