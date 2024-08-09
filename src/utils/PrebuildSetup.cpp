#include <iostream>

#include "GenerateShaders.h"
#include "GenerateKernels.h"

const std::string disclaimer = "// DO NOT EDIT: GENERATED FILE";

#ifdef _WIN32
    std::filesystem::path shadersPath = "../../../resources/shaders";
    std::filesystem::path kernelsPath = "../../../resources/kernels";
    std::filesystem::path headerPath = "../../../resources/include";
#else
    std::filesystem::path shadersPath = "../resources/shaders";
    std::filesystem::path kernelsPath = "../resources/kernels";
    std::filesystem::path headerPath = "../resources/include";
#endif

int main() {
    int ret = 0;
    if (GenerateShaders(disclaimer, shadersPath, headerPath)) {
        std::cerr << "shaders generation failed" << std::endl;
        ret = 1;
    }

    if (GenerateKernels(disclaimer, kernelsPath, headerPath)) {
        std::cerr << "kernels generation failed" << std::endl;
        ret = 1;
    }

    return ret;
}
