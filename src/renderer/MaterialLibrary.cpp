#include "MaterialLibrary.h"
#include <unordered_map>

namespace GoL {

std::unordered_map<std::string, Material*> MaterialLibrary::materials = std::unordered_map<std::string, Material*>();
std::vector<std::string> MaterialLibrary::names = std::vector<std::string>();

bool MaterialLibrary::AddMaterial(std::pair<std::string, Material*> entry) {
    if (materials.contains(entry.first)) {
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
