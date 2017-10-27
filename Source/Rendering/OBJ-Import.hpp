#ifndef SWAN_OBJ_IMPORT_HPP
#define SWAN_OBJ_IMPORT_HPP

#include "Mesh.hpp"               // For Mesh
#include "Physics/ColWrapper.hpp" // For ColWrapper::Type
#include <memory>                 // For std::unique_ptr<T>, std::make_unique<T>()
#include <string>                 // For std::string

namespace SWAN {
namespace Import {
	struct Settings {
		bool             smoothNormals = false;
		bool             createAABB    = false;
		ColWrapper::Type colType       = ColWrapper::COL_NONE;
	};

	std::unique_ptr<Mesh> OBJ (std::string filename, Settings s = Settings ());
}
}

#endif //OBJ_IMPORT_HPP
