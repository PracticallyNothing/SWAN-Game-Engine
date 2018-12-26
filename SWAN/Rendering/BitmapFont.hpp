#ifndef SWAN_BITMAP_FONT_HPP
#define SWAN_BITMAP_FONT_HPP

#include <array>  // For std::array<N, T>
#include <cctype> // For std::isprint()
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "Image.hpp"   // For Image
#include "Texture.hpp" // For Texture

#include "Physics/Transform.hpp" // For SWAN::Transform

namespace SWAN {
    /// A structure holding a font.
    /// @warn This class supports only monospace fonts.
    class BitmapFont {
    public:
	/// Load a bitmap from a configuration file.
	explicit BitmapFont(const std::string& confFilename);
	~BitmapFont();

	/// Get the width in pixels of an ASCII glyph.
	int getGlyphWidth(char c = ' ') const;

	/// Get the height in pixels of the font.
	int getGlyphHeight() const { return glyphHeight; }

	/// Get the width of a string of text.
	int getTextWidth(std::string text) const;

	/// Get the height of a string of text.
	int getTextHeight(std::string text) const;

	/// Get how many glyphs there are per row in the font's texture.
	int getGlyphsPerRow() const { return glyphsPerRow; }

	/// Can a character be rendered?
	bool isSupported(char c) const {return supportedChars.find(c) != std::string::npos;}

	/// Width of the tab character.
	int tabWidth;

	/// Get the texture of the font.
	inline const Texture* getTexture() const { return tex; }

	/// Get the UVs for a certain glyph.
	std::array<vec2, 4> getGlyphUVs(char c) const;

	/// Get the width of the font's texture in OpenGL texture units.
	double getGLWidth() const { return (double) glyphWidth / img->width; }
	/// Get the height of the font's texture in OpenGL texture units.
	double getGLHeight() const { return (double) glyphHeight / img->height; }

    private:
	Image* img;
	Texture* tex;

	std::string supportedChars;

	int glyphWidth,
	    glyphHeight,
	    glyphsPerRow;
    };
} // namespace SWAN

#endif // BITMAP_FONT_HPP
