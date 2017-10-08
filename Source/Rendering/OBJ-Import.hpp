#ifndef OBJ_IMPORT_HPP
#define OBJ_IMPORT_HPP

#include "Mesh.hpp" // For Mesh
#include <string>   // For std::string
#include <memory>   // For std::unique_ptr<T>, std::make_unique<T>()
#include "../Physics/ColWrapper.hpp" // For ColWrapper::Type

namespace Import {
    struct Settings {
        bool smoothNormals = false;
        bool createAABB = false;
		ColWrapper::Type colType = ColWrapper::COL_NONE;
	};

    std::unique_ptr<Mesh> OBJ(std::string filename, Settings s = Settings());
}

#endif //OBJ_IMPORT_HPP
