#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

const std::string disclaymer = "// DO NOT EDIT: GENERATED FILE";

int main() {
    std::filesystem::path resources_path = "../resources";
    std::filesystem::path hdr_path = "../resources/include";
    std::filesystem::directory_entry resources_dir(resources_path);

    if (!resources_dir.exists() || !resources_dir.is_directory()) {
        std::cerr << "failed to open dir: " << resources_path << std::endl;
        return 1;
    }

    if (!std::filesystem::directory_entry(hdr_path).exists()) {
        std::filesystem::create_directory(hdr_path);
    }


    for (auto& dir_entry : std::filesystem::recursive_directory_iterator(resources_path)) {
        if (!dir_entry.is_regular_file()) {
            // std::cerr << "skipping non regular file: " << dir_entry << std::endl;
            continue;
        }

        if (!dir_entry.path().string().contains(".shader")) {
            continue;
        }

        auto out_name = dir_entry.path().string();
        for (int i = out_name.size() - 1; i > 0; i--) {
            if (std::string(out_name.c_str() + i) == ".shader") {
                out_name.resize(i);
                break;
            }
        }
        for (int i = 0; i < out_name.size(); i++) {
            if (!std::string(out_name.c_str() + i).contains("/")) {
                out_name = std::string(out_name.c_str() + i);
                out_name = "shader_" + out_name;
            }
        }

        std::cout << out_name << std::endl;

        auto src = std::ifstream(dir_entry.path());
        if (!src.is_open()) {
            std::cerr << "failed to open file: " << dir_entry << std::endl;
            continue;
        }

        auto out = std::ofstream(hdr_path.string() + "/" + out_name + ".h", std::ios::out);
        if (!out.is_open()) {
            std::cerr << "failed to open file for write: " << hdr_path.string() + "/" + out_name + ".h" << std::endl;
            continue;
        }
        out <<
            disclaymer +
            "\n#pragma once\n#include <string>\n\nconst std::string __" +
            out_name +
            " = \"\\\n";

        std::string line;
        while (std::getline(src, line)) {
            out << line + "\\n\\\n";
        }
        out << "\";";

        out.close();
    }

    return 0;

}