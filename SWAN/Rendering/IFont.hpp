#ifndef SWAN_IFONT_HPP
#define SWAN_IFONT_HPP

#include "Core/Defs.hpp"
#include "Texture.hpp"
#include "Utility/UTF-8.hpp"

namespace SWAN
{
	class IFont
	{
	  public:
		virtual ~IFont() {}

		/// Get the width in pixels of an ASCII glyph.
		virtual int GetGlyphWidth(UTF8Char c) const = 0;

		/// Get the height in pixels of the font.
		virtual int GetGlyphHeight(UTF8Char c) const = 0;

		virtual bool SupportsChar(UTF8Char c) const = 0;

		/// Get the width of a string of text.
		size_t GetTextWidth(UTF8String text) const
		{
			size_t maxSize = 0, currSize = 0;
			for(size_t i = 0; i < text.length(); i++) {
				if(text[i] == '\n') {
					maxSize = std::max(maxSize, currSize);
					currSize = 0;
				} else {
					currSize += GetGlyphWidth(text[i]);
				}
			}
			maxSize = std::max(maxSize, currSize);
			return maxSize;
		}

		/// Get the height of a string of text.
		size_t GetTextHeight(String text) const
        {
		}

		/// Width of the tab character.
		virtual int GetTabWidth() { return 4; }

		/// Get the UVs for a certain glyph.
		virtual Array<vec2, 4> GetGlyphUVs(char c) const = 0;

		/// Get the width of the font's texture in OpenGL texture units.
		virtual double GetGLWidth() const = 0;

		/// Get the height of the font's texture in OpenGL texture units.
		virtual double GetGLHeight() const = 0;
	};
} // namespace SWAN

#endif