#ifndef SWAN_BITMAP_FONT_HPP
#define SWAN_BITMAP_FONT_HPP

#include <cctype> // For std::isprint()
#include <string> // For std::string
#include <vector> // For std::vector<T>

#include "Image.hpp"   // For Image
#include "Texture.hpp" // For Texture

#include "Utility/Debug.hpp" // For SWAN_DEBUG_VAR(), SWAN_DEBUG_DO()

namespace SWAN {

#ifdef SWAN_DEBUG
struct dbg_TextureCoords {
	int x, y, width, height;
};

inline std::ostream& operator<<(std::ostream& os, dbg_TextureCoords d) {
	os << "dbg_TextureCoords { x: " << d.x << ", y: " << d.y
	   << ", width: " << d.width << ", height: " << d.height << " }";
	return os;
}
#endif

// SUPPORTS ONLY MONOSPACE FONTS
class BitmapFont {
	SWAN_DEBUG_USING(dbg_TexCoordsVec, std::vector<dbg_TextureCoords>);

  public:
	explicit BitmapFont(const std::string& confFilename);
	~BitmapFont();

	const Texture* getGlyphTexture(char c, bool bold = false,
	                               bool italics = false) const {
		if(hasBoldItalics() && bold && italics)
			return &boldItalicsGlyphs.at(c - '!');
		else if(hasBold() && bold)
			return &boldGlyphs.at(c - '!');
		else if(hasItalics() && italics)
			return &italicsGlyphs.at(c - '!');
		else
			return &glyphs.at(c - '!');
	}

	Transform getGlyphUVTransform(char c) const;

	int getGlyphWidth(char c = ' ') const;
	int getGlyphHeight() const { return glyphHeight; }

	int getTextWidth(std::string text) const;
	int getTextHeight(std::string text) const;

	bool isSupported(char c) const {
		return supportedChars.find(c) != std::string::npos;
	}

	bool hasBold() const { return boldImg; }
	bool hasItalics() const { return italicsImg; }
	bool hasBoldItalics() const { return boldItalicsImg; }

	int tabWidth;

	SWAN_DEBUG_VAR(Texture*, dbg_tex);
	SWAN_DEBUG_VAR(dbg_TexCoordsVec, dbg_coords);

	inline const Texture* getTexture() const { return tex; }

  private:
	void genGlyphs();

	Image *img, *boldImg, *italicsImg, *boldItalicsImg;
	Texture* tex;

	std::vector<Texture> glyphs, boldGlyphs, italicsGlyphs, boldItalicsGlyphs;

	std::string supportedChars;

	int glyphWidth, glyphHeight,
	    glyphsPerRow;
};
}

#endif // BITMAP_FONT_HPP
