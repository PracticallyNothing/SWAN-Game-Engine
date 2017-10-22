#ifndef SWAN_RESOURCES_HPP
#define SWAN_RESOURCES_HPP

//#include <unordered_map> // For std::unordered_map<K, V>
#include <map>     // For std::map<K, V>
#include <memory>  // For std::unique_ptr<T>
#include <string>  // For std::string
#include <vector>  // For std::vector<T>

#include "Rendering/BitmapFont.hpp"  // For BitmapFont
#include "Rendering/Mesh.hpp"        // For Mesh
#include "Rendering/Shader.hpp"      // For Shader
#include "Rendering/Texture.hpp"     // For Texture

namespace SWAN {
namespace Res {
	using String = std::string;
	using CStrRef = const String&;

	template <typename T>
		using Pointer = std::unique_ptr<T>;

	template <typename K, typename V>
		using Map = std::map<K, V>;

	template<typename T>
		using Vector = std::vector<T>;

    namespace detail {
        extern Map<String, Pointer<Mesh>> meshes;
        extern Map<String, Pointer<Texture>> textures;
        extern Map<String, Pointer<BitmapFont>> bitmapFonts;
        extern Map<String, Pointer<Shader>> shaders;
    }

	enum LoadStatus {
		L_OK,
		L_NAMETAKEN,
		L_FILEMISSING,
		L_ERR,
	};

	extern LoadStatus LoadShader     (CStrRef filename, CStrRef name, Vector<String> attribs, Vector<String> uniforms);
    extern LoadStatus LoadBitmapFont (CStrRef filename, CStrRef name);
    extern LoadStatus LoadMesh       (CStrRef filename, CStrRef name);
    extern LoadStatus LoadTexture    (CStrRef filename, CStrRef name, bool pixelated = false);

    bool LoadFromFile(const std::string& filename);

    const Mesh* GetMesh(const std::string& name);
    const Texture* GetTexture(const std::string& name);
    const BitmapFont* GetBitmapFont(const std::string& name);
    Shader* GetShader(const std::string& name);
}
}

#endif
