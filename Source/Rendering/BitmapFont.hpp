#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include <string>  // For std::string
#include <cctype>  // For std::isprint()
#include <vector>  // For std::vector<T>

#include "Image.hpp"   // For Image
#include "Texture.hpp" // For Texture

#include "../Utility/Debug.hpp" // For DEBUG_VAR(), DEBUG_DO()

#ifdef DEBUG
struct dbg_TextureCoords { int x, y, width, height; };
inline std::ostream& operator<<(std::ostream& os, dbg_TextureCoords d){
	os << "dbg_TextureCoords { x: " << d.x << 
							", y: " << d.y << 
							", width: " << d.width << 
							", height: " << d.height << " }";
	return os;
}
#endif

// SUPPORTS ONLY MONOSPACE FONTS
class BitmapFont {
	DEBUG_USING(dbg_TexCoordsVec, std::vector<dbg_TextureCoords>);

	public:
		explicit BitmapFont(const std::string& confFilename);
		~BitmapFont();
		
		const Texture* getGlyphTexture(char c, bool bold = false, bool italics = false) const { 
			if(hasBoldItalics() && bold && italics)
				return &boldItalicsGlyphs.at(c - '!');
			else if(hasBold() && bold)
				return &boldGlyphs.at(c - '!');
			else if(hasItalics() && italics)
				return &italicsGlyphs.at(c - '!');
			else
				return &glyphs.at(c - '!'); 
		}

		int getGlyphWidth(char c = ' ') const;
		int getGlyphHeight() const { return glyphHeight; }
		
		bool isSupported(char c) const { return supportedChars.find(c) != std::string::npos; }
		
		bool hasBold() const { return boldImg; }
		bool hasItalics() const { return italicsImg; }
		bool hasBoldItalics() const { return boldItalicsImg; }
		
		int tabWidth;
	
		DEBUG_VAR(Texture*, dbg_tex);
		DEBUG_VAR(dbg_TexCoordsVec, dbg_coords);
	private:
		void genGlyphs();
		
		Image *img, *boldImg, *italicsImg, *boldItalicsImg;

		std::vector<Texture> 
			glyphs,
			boldGlyphs,
			italicsGlyphs,
			boldItalicsGlyphs;
		
		std::string supportedChars;
		
		int glyphWidth;
		int glyphHeight;
};

#endif //BITMAP_FONT_HPP
