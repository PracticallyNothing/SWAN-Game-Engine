#include "BitmapFont.hpp"

#include "External/stb_image.h"

#include "Utility/Math.hpp"
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()

#include <cpptoml.h> // For cpptoml::parse_file()
#include <iostream>  // For std::cout

const std::string defSuppChars =
    " !\"#$%&'()*+,-./"
    "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ["
    "\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

namespace SWAN {
// TODO: Move to importer function
BitmapFont::BitmapFont(const std::string& confFilename)
    : boldImg(nullptr), italicsImg(nullptr), boldItalicsImg(nullptr) {
	auto conf           = cpptoml::parse_file(confFilename);
	auto confGlyphWidth = conf->get_as<int>("glyph_width");

	if(!confGlyphWidth) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename
		          << "\" doesn't contain a \"glyph_width\" field!\n";
		return;
	} else if(*confGlyphWidth < 4) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has too small of a \"glyph_width\" field!\n";
		return;
	} else {
		glyphWidth = *confGlyphWidth;
	}

	auto confGlyphHeight = conf->get_as<int>("glyph_height");
	if(!confGlyphHeight) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename
		          << "\" doesn't contain a \"glyph_height\" field!\n";
		return;
	} else if(*confGlyphHeight < 4) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename
		          << "\" has too small of a \"glyph_height\" field!\n";
		return;
	} else {
		glyphHeight = *confGlyphHeight;
	}

	auto confImageName = conf->get_as<std::string>("image_file");
	if(!confImageName) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename
		          << "\" doesn't contain a \"image_file\" field!\n";
		return;
	}

	std::string dir = "";
	if(Util::IsRelativePath(confFilename))
		dir = SWAN::Util::GetDirectory(confFilename);

	stbi_set_flip_vertically_on_load(true);
	img = new Image((dir + *confImageName).c_str());
	if(!img->data) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename
		          << "\" has an incorrect \"image_file\" field!\n";
		return;
	}
	stbi_set_flip_vertically_on_load(false);

	auto confImgBold = conf->get_as<std::string>("image_bold_file");
	if(confImgBold) {
		boldImg = new Image((dir + *confImgBold).c_str());
		if(!boldImg->isValid()) {
			delete boldImg;
			boldImg = nullptr;
		}
	}

	auto confImgItalics = conf->get_as<std::string>("image_italics_file");
	if(confImgItalics) {
		italicsImg = new Image((dir + *confImgItalics).c_str());
		if(!italicsImg->isValid()) {
			delete italicsImg;
			italicsImg = nullptr;
		}
	}

	auto confImgBoldItalics =
	    conf->get_as<std::string>("image_bold_italics_file");
	if(confImgBoldItalics) {
		boldItalicsImg = new Image((dir + *confImgBoldItalics).c_str());
		if(!boldItalicsImg->isValid()) {
			delete boldItalicsImg;
			boldItalicsImg = nullptr;
		}
	}

	std::cout << "Success: Bitmap font config file\"" << confFilename
	          << "\" has been loaded correctly!\n";

	supportedChars =
	    conf->get_as<std::string>("supported_chars").value_or(defSuppChars);
	tabWidth = conf->get_as<int>("tab_width").value_or(4);

	glyphsPerRow = img->width / glyphWidth;
	SWAN_DEBUG_OUT(glyphsPerRow);

	tex = new Texture(*img);

	genGlyphs();
}

int BitmapFont::getGlyphWidth(char c) const {
	if(std::isprint(c)) {
		return glyphWidth;
	} else if(c == '\t') {
		return tabWidth * glyphWidth;
	} else {
		return 0;
	}
}

void BitmapFont::genGlyphs() {
	char c = '!';
	for(int y = 0; y < img->height / glyphHeight; y++) {
		for(int x = 0; x < img->width / glyphWidth; x++) {
			if(c > '~')
				break;

			int xx = x * glyphWidth;
			int yy = y * glyphHeight;

			SWAN_DEBUG_DO(
			    dbg_coords.push_back({ xx, yy, glyphWidth, glyphHeight }));

			glyphs.push_back(
			    Texture(img->subImg(xx, yy, glyphWidth, glyphHeight)));

			if(boldImg)
				boldGlyphs.push_back(
				    Texture(boldImg->subImg(xx, yy, glyphWidth, glyphHeight)));

			if(italicsImg)
				italicsGlyphs.push_back(Texture(
				    italicsImg->subImg(xx, yy, glyphWidth, glyphHeight)));

			if(boldItalicsImg)
				boldItalicsGlyphs.push_back(Texture(
				    boldItalicsImg->subImg(xx, yy, glyphWidth, glyphHeight)));

			c++;
		}
	}
	SWAN_DEBUG_DO(dbg_tex = new Texture(*img));
}

Transform BitmapFont::getGlyphUVTransform(char c) const {
	Transform transform;

	if(c == ' ' || c == '\t' || c == '\n') {
		transform.pos   = glm::vec3(0, 0, 0);
		transform.scale = glm::vec3(0, 0, 0);
		return transform;
	}

	transform.scale.x = (float) glyphWidth / img->width;
	transform.scale.y = (float) glyphHeight / img->height;

	int glyphX = ((c - '!') % glyphsPerRow) * glyphWidth,
	    glyphY = ((c - '!' + glyphsPerRow) / glyphsPerRow) * glyphHeight;

	transform.pos.x = Util::Normalize(glyphX, 0, img->width);
	transform.pos.y = 1.0 - Util::Normalize(glyphY, 0, img->height);

	return transform;
}

BitmapFont::~BitmapFont() {
	SWAN_DEBUG_DO(delete dbg_tex);
	delete img;
	delete tex;
}
} // namespace SWAN
