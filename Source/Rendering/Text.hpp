#ifndef SWAN_TEXT_HPP
#define SWAN_TEXT_HPP

#include "BitmapFont.hpp" // For BitmapFont
#include "Mesh.hpp"       // For Mesh
#include "Shader.hpp"     // For Shader
#include "Texture.hpp"    // For Texture

#include "Physics/Transform.hpp"

#include "Utility/Debug.hpp" // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR

#include <string> // For std::string
namespace SWAN {
void RenderText(
    int x, int y,
    std::string text,
    Shader* s,
    const BitmapFont* f,
    SWAN::Color color   = SWAN::Color{ 0, 0, 0, 255 },
    SWAN::Color bgColor = SWAN::Color{ 0, 0, 0, 0 });
}

#endif
