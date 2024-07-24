#include "precompiled.h"

#include "MaterialLibrary.h"

namespace GoL {

std::unordered_map<std::string, Material*> MaterialLibrary::materials = std::unordered_map<std::string, Material*>();
std::vector<std::string> MaterialLibrary::names = std::vector<std::string>();

bool MaterialLibrary::AddMaterial(std::pair<std::string, Material*> entry) {
    if (materials.contains(entry.first)) {
        CORE_WARN("Material with name {} attempted to be added twice", entry.first);
        delete entry.second;
        return false;
    } else {
        materials[entry.first] = entry.second;
        names.push_back(entry.first);
        return true;
    }
}

Material* MaterialLibrary::GetMaterial(const std::string& name) {
    if (materials.contains(name)) {
        return materials[name];
    } else {
        return nullptr;
    }
}

const std::vector<std::string>& MaterialLibrary::GetNames() {
    return names;
}

}
