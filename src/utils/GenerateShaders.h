#pragma once

#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

int GenerateShaders(std::string disclaimer, std::filesystem::path resourcesPath, std::filesystem::path headerPath) {
    std::filesystem::directory_entry resourcesDir(resourcesPath);

    if (!resourcesDir.exists() || !resourcesDir.is_directory()) {
        std::cerr << "failed to open dir: " << resourcesPath << std::endl;
        return 1;
    }

    if (!std::filesystem::directory_entry(headerPath).exists()) {
        std::filesystem::create_directory(headerPath);
    }


    for (auto& dirEntry : std::filesystem::recursive_directory_iterator(resourcesPath)) {
        if (!dirEntry.is_regular_file()) {
            // std::cerr << "skipping non regular file: " << dir_entry << std::endl;
            continue;
        }

        if (dirEntry.path().string().find(".shader") == std::string::npos) {
            continue;
        }

        auto outName = dirEntry.path().string();
        for (int i = outName.size() - 1; i > 0; i--) {
            if (outName.substr(i) == ".shader") {
                outName.resize(i);
                break;
            }
        }
        for (int i = 0; i < outName.size(); i++) {
            if (outName.substr(i).find("/") == std::string::npos) {
                outName = outName.substr(i);
                outName = "shader_" + outName;
            }
        }

        std::cout << outName << std::endl;

        auto src = std::ifstream(dirEntry.path());
        if (!src.is_open()) {
            std::cerr << "failed to open file: " << dirEntry << std::endl;
            continue;
        }

        auto out = std::ofstream(headerPath.string() + "/" + outName + ".h", std::ios::out);
        if (!out.is_open()) {
            std::cerr << "failed to open file for write: " << headerPath.string() + "/" + outName + ".h" << std::endl;
            continue;
        }

        std::for_each(outName.begin(), outName.end(), [](char& l) { l = std::toupper(l); });

        out <<
            disclaimer +
            "\n#pragma once\n#include <string>\n\nnamespace GoL {\n\nconst std::string " +
            outName +
            " = \"\\\n";

        std::string line;
        while (std::getline(src, line)) {
            out << line + "\\n\\\n";
        }
        out << "\";\n\n}";

        out.close();
    }

    return 0;
}