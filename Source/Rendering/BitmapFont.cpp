#include "BitmapFont.hpp"

#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()

#include "Core/Display.hpp" // For SWAN::Display

#include "Utility/Math.hpp"       // For SWAN::Util::Normalize()
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()

#include <algorithm> // For std::max_element(), std::count()
#include <cpptoml.h> // For cpptoml::parse_file()
#include <iostream>  // For std::cout

namespace SWAN {
// TODO: Move to importer function
BitmapFont::BitmapFont(const std::string& confFilename) {
	auto conf           = cpptoml::parse_file(confFilename);
	auto confGlyphWidth = conf->get_as<int>("glyph_width");

	if(!confGlyphWidth) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" doesn't contain a \"glyph_width\" field!\n";
		return;
	} else if(*confGlyphWidth < 4) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has too small of a \"glyph_width\" field!\n";
		return;
	} else {
		glyphWidth = *confGlyphWidth;
	}

	auto confGlyphHeight = conf->get_as<int>("glyph_height");
	if(!confGlyphHeight) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" doesn't contain a \"glyph_height\" field!\n";
		return;
	} else if(*confGlyphHeight < 4) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has too small of a \"glyph_height\" field!\n";
		return;
	} else {
		glyphHeight = *confGlyphHeight;
	}

	auto confImageName = conf->get_as<std::string>("image_file");
	if(!confImageName) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" doesn't contain a \"image_file\" field!\n";
		return;
	}

	std::string dir = "";
	if(Util::IsRelativePath(confFilename))
		dir = SWAN::Util::GetDirectory(confFilename);

	stbi_set_flip_vertically_on_load(true);
	img = new Image((dir + *confImageName).c_str());
	if(!img->data) {
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has an incorrect \"image_file\" field!\n";
		return;
	}
	stbi_set_flip_vertically_on_load(false);

	std::cout << "Success: Bitmap font config file\"" << confFilename << "\" has been loaded correctly!\n";
	tabWidth = conf->get_as<int>("tab_width").value_or(4);

	glyphsPerRow = img->width / glyphWidth;
	tex          = new Texture(*img);
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

std::array<glm::vec2, 4> BitmapFont::getGlyphUVs(char c) const {
	std::array<glm::vec2, 4> uvs;

	double glWidth  = (double) glyphWidth / img->width;
	double glHeight = (double) glyphHeight / img->height;

	int glyphX = ((c - '!') % glyphsPerRow) * glyphWidth,
	    glyphY = ((c - '!' + glyphsPerRow) / glyphsPerRow) * glyphHeight;

	uvs[0].x = Util::Normalize(glyphX, 0, img->width);
	uvs[0].y = 1.0 - Util::Normalize(glyphY, 0, img->height) + glHeight;

	uvs[1].x = uvs[0].x;
	uvs[1].y = uvs[0].y - glHeight;

	uvs[2].x = uvs[0].x + glWidth;
	uvs[2].y = uvs[0].y;

	uvs[3].x = uvs[0].x + glWidth;
	uvs[3].y = uvs[0].y - glHeight;

	return uvs;
}

BitmapFont::~BitmapFont() {
	delete img;
	delete tex;
}

int BitmapFont::getTextWidth(std::string text) const {
	auto v = Util::SplitOn(text, '\n');
	return glyphWidth * (*std::max_element(v.begin(), v.end(), [](std::string a, std::string b) { return a.length() < b.length(); })).length();
}
int BitmapFont::getTextHeight(std::string text) const {
	return glyphHeight * std::count(text.begin(), text.end(), '\n');
}
} // namespace SWAN
