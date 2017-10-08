#include "Image.hpp"

#define STBI_IMPLEMENTATION
#include "External/stb_image.h" // For stbi_load(), stbi_image_free()

#include <iostream>  // For std::cout
#include <algorithm> // For std::copy()

const Color Color::White {255, 255, 255, 255};
const Color Color::Black {  0,   0,   0, 255};
const Color Color::Red   {255,   0,   0, 255};
const Color Color::Green {  0, 255,   0, 255};
const Color Color::Blue  {  0,   0, 255, 255};

Image::Image() : data(nullptr), width(-1), height(-1), stbiLoaded(false) {}
Image::Image(int w, int h) : data(new uint8_t[w * h * 4]), width(w),  height(h),  stbiLoaded(false) {}
Image::Image(const Image& other) 
	: data(new uint8_t[other.width * other.height * 4]), 
	  width(other.width), 
	  height(other.height), 
	  stbiLoaded(false) 
{
	std::copy(other.data, other.data + width*height*4, data);
}

Image& Image::operator=(const Image& other) {
	data = new uint8_t[other.width * other.height * 4];
	width = other.width;
	height = other.height;
	stbiLoaded = false;
	std::copy(other.data, other.data + width*height*4, data);
	
	return *this;
}

Image::Image(const char* filename) : stbiLoaded(true) {
	int nComp;
	data = stbi_load(filename, &width, &height, &nComp, 4);
	
	if(!data)
		std::cout << "ERROR: Image \"" << filename << "\" could not be loaded!\n";
}

Image Image::subImg(int xOffs, int yOffs, int w, int h) const {
	if(xOffs < 0 ||	yOffs < 0 || xOffs + w > width || yOffs + h > width){
		std::cout << "ERROR( Image::subImg(xOffs = " << xOffs 
									<< ", yOffs = " << yOffs
				  					<< ", w = " << w 
									<< ", h = " << h
				  					<< ", this->width = " << this->width
									<< ", this->height = " << this->height
									<< "): Attempt to create Image from non-existant pixels.\n";
		return Image();
	}

	Image res(w, h);
	
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			res.data[x*4 + y*w*4    ] = data[(x+xOffs)*4 + (y+yOffs)*width*4    ]; // Alpha
			res.data[x*4 + y*w*4 + 1] = data[(x+xOffs)*4 + (y+yOffs)*width*4 + 1]; // Red
			res.data[x*4 + y*w*4 + 2] = data[(x+xOffs)*4 + (y+yOffs)*width*4 + 2]; // Green
			res.data[x*4 + y*w*4 + 3] = data[(x+xOffs)*4 + (y+yOffs)*width*4 + 3]; // Blue
		}
	}
	
	return res;
}

Color Image::pixelAt(unsigned x, unsigned y) const {
	if(x > (unsigned)width || y > (unsigned)height){
		return Color{0, 0, 0, 0};
	}
	return Color{data[x*4 + y*width*4], 	 // Red
				 data[x*4 + y*width*4 + 1],  // Green
				 data[x*4 + y*width*4 + 2],  // Blue
				 data[x*4 + y*width*4 + 3]}; // Alpha
}

dbg_ColorImg Image::dbg_toColorImg() const {
	Color* res = new Color[width * height];
	
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			res[x + y * width] = pixelAt(x, y);
		}
	}

	return {res, width, height};
}

Image::~Image(){
	if(stbiLoaded)
		stbi_image_free(data);
	else
		delete[] data;
}
