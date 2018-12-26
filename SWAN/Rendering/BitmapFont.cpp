#include "BitmapFont.hpp"

#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()

#include "Core/Display.hpp" // For SWAN::Display

#include "Utility/INI.hpp"       // For SWAN::Util::Normalize()
#include "Utility/Math.hpp"       // For SWAN::Util::Normalize()
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()

#include <algorithm>                             // For std::max_element(), std::count()
#include <iostream>                              // For std::cout

namespace SWAN {
    // TODO: Move to importer function
    BitmapFont::BitmapFont(const std::string& confFilename) {
	auto conf = Util::INI::ParseFile(confFilename);
	if(!conf.hasVar("glyph_width"))
	{
	    return;
	}
	else if(Util::INI::ToInt(conf["global"]["glyph_width"]) < 4)
	{
	    return;
	}
	else
	{
	    glyphWidth = Util::INI::ToInt(conf["global"]["glyph_width"]);
	}

	if(!conf.hasVar("glyph_height"))
	{
	    return;
	}
	else if(Util::INI::ToInt(conf["global"]["glyph_height"]) < 4)
	{
	    return;
	}
	else
	{
	    glyphHeight = Util::INI::ToInt(conf["global"]["glyph_height"]);
	}

	std::string confImageName; 

	if(!conf.hasVar("image_file"))
	{
	    return;
	}
	else
	{
	    confImageName = conf["global"]["image_file"];
	}

	std::string dir = "";
	if(Util::IsRelativePath(confFilename))
	    dir = SWAN::Util::GetDirectory(confFilename);

	stbi_set_flip_vertically_on_load(true);
	img = new Image((dir + confImageName).c_str());
	if(!img->data)
	{
	    std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has an incorrect \"image_file\" field!\n";
	    return;
	}
	stbi_set_flip_vertically_on_load(false);

	//std::cout << "Success: Bitmap font config file\"" << confFilename << "\" has been loaded correctly!\n";
	tabWidth = (conf.hasVar("tab_width") ? Util::INI::ToInt(conf["global"]["tab_width"]) : 4);

	glyphsPerRow = img->width / glyphWidth;
	tex = new Texture(*img);
    }

    int BitmapFont::getGlyphWidth(char c) const
    {
	if(std::isprint(c))
	    return glyphWidth;
	else if(c == '\t')
	    return tabWidth * glyphWidth;
	else
	    return 0;
    }

    std::array<vec2, 4> BitmapFont::getGlyphUVs(char c) const
    {
	std::array<vec2, 4> uvs;

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

    BitmapFont::~BitmapFont()
    {
	delete img;
	delete tex;
    }

    int BitmapFont::getTextWidth(std::string text) const {
	if(!text.length())
	    return 0;

	auto v = Util::SplitOn(text, '\n');

	return glyphWidth *
	    std::max_element(
		v.begin(), v.end(),
		[](const std::string& a, const std::string& b) { return a.length() < b.length(); }
	    )->length();
    }

    int BitmapFont::getTextHeight(std::string text) const
    {
	if(!text.length())
	    return 0;

	return glyphHeight * (1 + std::count(text.begin(), text.end(), '\n'));
    }
} // namespace SWAN
