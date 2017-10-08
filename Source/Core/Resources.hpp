#ifndef RESOURCES_HPP
#define RESOURCES_HPP

//#include <unordered_map> // For std::unordered_map<K, V>
#include <map>     // For std::map<K, V>
#include <memory>  // For std::unique_ptr<T>
#include <string>  // For std::string

#include "../Rendering/BitmapFont.hpp"  // For BitmapFont
#include "../Rendering/Mesh.hpp"        // For Mesh
#include "../Rendering/Shader.hpp"      // For Shader
#include "../Rendering/Texture.hpp"     // For Texture

namespace Resources {
    namespace detail {
        using String = std::string;

        template <typename T>
        using Pointer = std::unique_ptr<T>;

        template <typename K, typename V>
        using Map = std::map<K, V>;

        extern Map<String, Pointer<Mesh>> meshes;
        extern Map<String, Pointer<Texture>> textures;
        extern Map<String, Pointer<BitmapFont>> bitmapFonts;
        extern Map<String, Pointer<Shader>> shaders;
    }
    
    bool LoadShader(const std::string& filename);
    bool LoadBitmapFont(const std::string& filename);
    bool LoadMesh(const std::string& filename);
    bool LoadTexture(const std::string& filename, bool pixelated = false);
    
    bool LoadFromFile(const std::string& filename);

    const Mesh* GetMesh(const std::string& name);
    const Texture* GetTexture(const std::string& name);
    const BitmapFont* GetBitmapFont(const std::string& name);
    Shader* GetShader(const std::string& name);
}

#endif
