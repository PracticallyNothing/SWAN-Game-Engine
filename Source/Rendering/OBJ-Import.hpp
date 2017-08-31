#ifndef OBJ_IMPORT_HPP
#define OBJ_IMPORT_HPP

#include "Mesh.hpp" // For Mesh
#include <string>   // For std::string
#include <memory>   // For std::unique_ptr<T>, std::make_unique<T>()

namespace Import {
    struct Settings {
        bool smoothNormals = false;
        bool createAABB = false;
    };

    std::unique_ptr<Mesh> OBJ(std::string filename, Settings s = Settings());
}

#endif //OBJ_IMPORT_HPP
