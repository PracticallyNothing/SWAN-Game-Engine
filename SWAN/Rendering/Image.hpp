#ifndef SWAN_IMAGE_HPP
#define SWAN_IMAGE_HPP

#include "External/stb_image.h"

#include "Maths/Vector.hpp"

#include <cstdint>  // For std::uint8_t
#include <iomanip>  // For std::setw()
#include <ostream>
#include <iostream> // For std::ostream, std::operator<<()

namespace SWAN {
    /// A structure for holding colors.
    struct Color {
	uint8_t red, green, blue, alpha;

	static const Color White, Black, Red, Green, Blue;

	Color() : red(0), green(0), blue(0), alpha(0) {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a) {}

	explicit operator vec3() const {
	    return vec3((double) red / 255.0, (double) green / 255.0,
			(double) blue / 255.0);
	}
	explicit operator vec4() const {
	    return vec4((double) red / 255.0, (double) green / 255.0,
			(double) blue / 255.0, (double) alpha / 255.0);
	}
    };

    /// A representation of an Image from colors. For debugging purposes.
    struct dbg_ColorImg {
	~dbg_ColorImg() { delete data; }

	Color* data;
	int width, height;
    };

    inline std::ostream& operator<<(std::ostream& os, dbg_ColorImg i) {
	os << "dbg_ColorImg (" << i.width << "x" << i.height << "){\n";
	for(int y = 0; y < i.height; y++) {
	    for(int x = 0; x < i.width; x++) {
		os << "{ " << std::setw(3) << (int) i.data[x + y * i.width].red
		   << ", " << std::setw(3) << (int) i.data[x + y * i.width].green
		   << ", " << std::setw(3) << (int) i.data[x + y * i.width].blue
		   << ", " << std::setw(3) << (int) i.data[x + y * i.width].alpha
		   << " }, ";
	    }
	    os << '\n';
	}
	os << '}';

	return os;
    }

    /// A class representing RGBA image data.
    struct Image {
	/// Create an empty image.
	Image();
	/// Create an image with a width and height.
	Image(int w, int h);
	/// Load an image from a file using stb_image.
	explicit Image(const char* filename);

	/// Copy constructor.
	Image(const Image& img);
	/// Move constructor.
	Image& operator=(const Image& img);

	~Image();

	/// Extract a rectangular part of an image.
	Image subImg(int x, int y, int w, int h) const;
	/// Get the pixel at the given coordinates as a SWAN::Color.
	Color pixelAt(unsigned x, unsigned y) const;

	/// Give the pixel at the given coordinate the value of a SWAN::Color.
	void setPixelAt(unsigned x, unsigned y, Color color);

	/// Convert the image data to an array of SWAN::Colors. For debugging.
	dbg_ColorImg to_dbg_ColorImg() const;

	/// Is the image valid?
	bool isValid() const { return data && width > 0 && height > 0; }

	/// Image data in an RGBA format.
	uint8_t* data;

	/// Size of the image.
	int width, height;

	/// Whether the image was loaded by stbi. Used in destructor.
	bool stbiLoaded;
    };
} // namespace SWAN

#endif
