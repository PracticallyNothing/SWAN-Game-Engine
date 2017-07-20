#include "BitmapFont.hpp"


#include "../External/cpptoml/cpptoml.h" // For cpptoml::parse_file()
#include <iostream> // For std::cout

const std::string defSuppChars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

BitmapFont::BitmapFont(const std::string& confFilename) 
	: boldImg(nullptr), italicsImg(nullptr), boldItalicsImg(nullptr)
{
	auto conf = cpptoml::parse_file(confFilename);

	auto confGlyphWidth = conf->get_as<int>("glyph_width");
	if(!confGlyphWidth)
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" doesn't contain a \"glyph_width\" field!\n";
	else if (*confGlyphWidth < 4)
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has too small of a \"glyph_width\" field!\n";
	else
		glyphWidth = *confGlyphWidth;

	auto confGlyphHeight = conf->get_as<int>("glyph_height");
	if(!confGlyphHeight)
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" doesn't contain a \"glyph_height\" field!\n";
	else if (*confGlyphHeight < 4)
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has too small of a \"glyph_height\" field!\n";
	else
		glyphHeight = *confGlyphHeight;	

	auto confImageName = conf->get_as<std::string>("image_file");
	if(!confImageName)
		std::cout << "ERROR: Bitmap font config file \"" 
			<< confFilename 
			<< "\" doesn't contain a \"image_file\" field!\n";

	img = new Image(confImageName->c_str());
	if(!img->data)
		std::cout << "ERROR: Bitmap font config file \"" << confFilename << "\" has an incorrect \"image_file\" field!\n";
	
	auto confImgBold = conf->get_as<std::string>("image_bold_file");
	if(confImgBold){
		boldImg = new Image(confImgBold->c_str());
		if(!boldImg->isValid()){
			delete boldImg;
			boldImg = nullptr;
		}
	}
	
	auto confImgItalics = conf->get_as<std::string>("image_italics_file");
	if(confImgItalics){
		italicsImg = new Image(confImgItalics->c_str());
		if(!italicsImg->isValid()){
			delete italicsImg;
			italicsImg = nullptr;
		}
	}

	auto confImgBoldItalics = conf->get_as<std::string>("image_bold_italics_file");
	if(confImgBoldItalics){
		boldItalicsImg = new Image(confImgBoldItalics->c_str());
		if(!boldItalicsImg->isValid()){
			delete boldItalicsImg;
			boldItalicsImg = nullptr;
		}
	}
	
	std::cout << "Success: Bitmap font config file\"" << confFilename << "\" has been loaded correctly!\n";

	supportedChars = conf->get_as<std::string>("supported_chars").value_or(defSuppChars);
	tabWidth = conf->get_as<int>("tab_width").value_or(4);

	genGlyphs();
}

int BitmapFont::getGlyphWidth(char c) const {
	if(std::isprint(c)){
		return glyphWidth;
	} else if(c == '\t') {
		return tabWidth * glyphWidth;
	} else {
		return 0;
	}
}

void BitmapFont::genGlyphs(){
	char c = '!';
	for(int y = 0; y < img->height / glyphHeight; y++){
		for(int x = 0; x < img->width / glyphWidth; x++){
			if(c > '~')
				break;
			
			int xx = x * glyphWidth;
			int yy = y * glyphHeight;

			DEBUG_DO(dbg_coords.push_back({xx, yy, glyphWidth, glyphHeight}));
			
			glyphs.push_back(Texture(img->subImg(xx, yy, glyphWidth, glyphHeight)));
			
			if(boldImg)
				boldGlyphs.push_back(Texture(boldImg->subImg(xx, yy, glyphWidth, glyphHeight)));

			if(italicsImg)
				italicsGlyphs.push_back(Texture(italicsImg->subImg(xx, yy, glyphWidth, glyphHeight)));
			
			if(boldItalicsImg)
				boldItalicsGlyphs.push_back(Texture(boldItalicsImg->subImg(xx, yy, glyphWidth, glyphHeight)));

			c++;
		}
	}
	DEBUG_DO(dbg_tex = new Texture(*img));
}

BitmapFont::~BitmapFont(){
	DEBUG_DO(delete dbg_tex);
	delete img;
}

