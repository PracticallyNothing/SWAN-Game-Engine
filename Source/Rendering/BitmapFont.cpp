#include "BitmapFont.hpp"

#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()

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

	std::cout << "Success: Bitmap font config file\"" << confFilename
	          << "\" has been loaded correctly!\n";
	tabWidth = conf->get_as<int>("tab_width").value_or(4);

	glyphsPerRow = img->width / glyphWidth;
	tex = new Texture(*img);
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
