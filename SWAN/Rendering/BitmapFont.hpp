#ifndef SWAN_BITMAP_FONT_HPP
#define SWAN_BITMAP_FONT_HPP

#include <array>  // For std::array<N, T>
#include <cctype> // For std::isprint()
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "Core/Defs.hpp"     // For String
#include "Image.hpp"         // For Image
#include "Maths/Vector.hpp"  // For vec2
#include "Texture.hpp"       // For Texture
#include "Utility/UTF-8.hpp" // For UTF8_*() functions.

namespace SWAN
{
	/// A structure holding a font.
	/// @warn This class supports only monospace fonts.
	class BitmapFont
	{
	  public:
		/// Initialize a BitmapFont from a given spritesheet.
		BitmapFont(int glyphWidth, int glyphHeight, Image* image, int tabWidth = 4);
		/// Destructor.
		~BitmapFont();

		/// Get the width in pixels of an ASCII glyph.
		int getGlyphWidth(char c = ' ') const;

		/// Get the height in pixels of the font.
		int getGlyphHeight() const { return glyphHeight; }

		/// Get the width of a string of text.
		int getTextWidth(String text) const;

		/// Get the height of a string of text.
		int getTextHeight(String text) const;

		/// Get how many glyphs there are per row in the font's texture.
		int getGlyphsPerRow() const { return glyphsPerRow; }

		/// Width of the tab character.
		int tabWidth = 4;

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

		int glyphWidth, glyphHeight;
		int glyphsPerRow;
	};

	/// Load a bitmap from an INI style configuration file.
	extern BitmapFont* ImportBitmapFromINI(const String& file);
} // namespace SWAN

#endif // BITMAP_FONT_HPP
