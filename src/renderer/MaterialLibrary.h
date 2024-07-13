#ifndef MATERIALLIBRARY_H_
#define MATERIALLIBRARY_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// #include "Materials/Material.h"



namespace GoL {

class Material;

class MaterialLibrary {
protected:
    static std::unordered_map<std::string, Material*> materials;
    static std::vector<std::string> names;

public:
    MaterialLibrary() = delete;
    MaterialLibrary(const MaterialLibrary& other) = delete;
    MaterialLibrary(MaterialLibrary&& other) = delete;

    ~MaterialLibrary();

    static bool AddMaterial(std::pair<std::string, Material*> entry);
    static Material* GetMaterial(const std::string& name);
    static const std::vector<std::string>& GetNames();
};

}
#endif // MATERIALLIBRARY_H_
