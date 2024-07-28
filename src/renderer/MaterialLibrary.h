#ifndef MATERIALLIBRARY_H_
#define MATERIALLIBRARY_H_

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace GoL {

class Material;
class MaterialPointer;

class MaterialLibrary {
protected:
    static std::unordered_map<std::string, Material*> materials;
    static std::vector<std::string> names;

public:
    MaterialLibrary() = delete;
    MaterialLibrary(const MaterialLibrary& other) = delete;
    MaterialLibrary(MaterialLibrary&& other) = delete;

    ~MaterialLibrary() = default;

    /**
    * @name AddMaterial - Adds material to the library
    * @param entry -  Entry is a pair of material name and a pointer to the Material
    * @return bool - True if Material is added and false otherwise
    */
    static bool AddMaterial(std::pair<std::string, Material*> entry);

    /**
    * @name GetMaterial - Looks for the given of type Material by name
    * @param name - Name of Material to be searched
    * @return MaterialPointer - Pointer to the Material (nullptr if not found). This pointer has no ownership of data and should not be deleted by user
    */
    static MaterialPointer GetMaterial(const std::string& name);

    /**
    * @name GetNames - Gives all names of Materials currently at the library
    * @return std::vector - collection of all Material names
    */
    static const std::vector<std::string>& GetNames();
};

}
#endif // MATERIALLIBRARY_H_
