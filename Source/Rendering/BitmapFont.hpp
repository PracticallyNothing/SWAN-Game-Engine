#ifndef SWAN_BITMAP_FONT_HPP
#define SWAN_BITMAP_FONT_HPP

#include <cctype> // For std::isprint()
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "Image.hpp"   // For Image
#include "Texture.hpp" // For Texture

#include "Physics/Transform.hpp" // For SWAN::Transform

namespace SWAN {

// SUPPORTS ONLY MONOSPACE FONTS
class BitmapFont {
  public:
	explicit BitmapFont(const std::string& confFilename);
	~BitmapFont();

	Transform getGlyphUVTransform(char c) const;

	int getGlyphWidth(char c = ' ') const;
	int getGlyphHeight() const { return glyphHeight; }

	int getTextWidth(std::string text) const;
	int getTextHeight(std::string text) const;

	bool isSupported(char c) const {
		return supportedChars.find(c) != std::string::npos;
	}

	int tabWidth;

	inline const Texture* getTexture() const { return tex; }

  private:
	Image* img;
	Texture* tex;

	std::string supportedChars;

	int glyphWidth,
	    glyphHeight,
	    glyphsPerRow;
};
}

#endif // BITMAP_FONT_HPP
