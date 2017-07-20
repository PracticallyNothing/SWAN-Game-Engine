#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../External/stb_image.h"

#include <cstdint> // For std::uint8_t
#include <iostream> // For std::ostream, std::operator<<()
#include <iomanip>    // For std::setw()
#include <glm/glm.hpp> // For glm::vec3, glm::vec4

struct Color {
	uint8_t red,
			green,
			blue,
			alpha;
	
	static const Color
			White,
			Black,
			Red,
			Green,
			Blue;
	
	explicit operator glm::vec3() const {
		return 
		glm::vec3((double)red   / 255.0,
				  (double)green / 255.0,
				  (double)blue  / 255.0);
	}
	explicit operator glm::vec4() const {
		return 
		glm::vec4((double)red   / 255.0,
				  (double)green / 255.0,
				  (double)blue  / 255.0,
				  (double)alpha / 255.0);
	}
};

struct dbg_ColorImg { 
	~dbg_ColorImg(){ delete data; }

	Color* data; 
	int width, height; 
};

inline std::ostream& operator<<(std::ostream& os, dbg_ColorImg i){
	os << "dbg_ColorImg (" << i.width << "x" << i.height << "){\n"; 
	for(int y = 0; y < i.height; y++) {
		for(int x = 0; x < i.width; x++) {
			os << "{ "   << std::setw(3) << (int) i.data[x + y * i.width].red
			   <<   ", " << std::setw(3) << (int) i.data[x + y * i.width].green
			   <<   ", " << std::setw(3) << (int) i.data[x + y * i.width].blue
			   <<   ", " << std::setw(3) << (int) i.data[x + y * i.width].alpha << " }, ";
		}
		os << '\n';
	}
	os << '}';

	return os;
}

struct Image {
	Image();
	Image(int w, int h);
	explicit Image(const char* filename);

	Image(const Image& img);
	Image& operator=(const Image& img);

	Image subImg(int x, int y, int w, int h) const;
	Color pixelAt(unsigned x, unsigned y) const;
	
	dbg_ColorImg dbg_toColorImg() const;
	~Image();

	bool isValid() const { return data && width > 0 && height > 0; }

	uint8_t* data;
	int width, height;
	bool stbiLoaded;
};

#endif
