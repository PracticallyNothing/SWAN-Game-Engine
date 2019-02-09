#ifndef SWAN_OBJ_IMPORT_HPP
#define SWAN_OBJ_IMPORT_HPP

#include "Mesh.hpp" // For Mesh

#include <memory> // For std::unique_ptr<T>, std::make_unique<T>()
#include <string> // For std::string

namespace SWAN
{
	namespace Import
	{
		/// Collection of settings for importer.
		struct Settings {
			/// Should the normals be smoothed after import?
			bool smoothNormals = false;
		};

		/// Import a Wavefront OBJ file using SWAN's built-in importer.
		std::unique_ptr<Mesh> OBJ(std::string filename, Settings s = Settings());
	} // namespace Import
} // namespace SWAN

#endif // OBJ_IMPORT_HPP
