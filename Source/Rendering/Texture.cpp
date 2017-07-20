#include "Texture.hpp"
#include <iostream>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "../External/stb_image.h"

#include "../Utility/Debug.hpp"

const Texture* Texture::currBoundTex = nullptr;

Texture::Texture(const std::string& filename, bool isPixelated, int type) 
	: img(new Image(filename.c_str())), delImg(true), type(type)
{
	init(isPixelated);
}

Texture::Texture(const Image& img, bool isPixelated, int type) 
	: img(&img), delImg(false), type(type)
{
	init(isPixelated);
}

Texture::Texture(Texture&& t) 
	: img(std::move(t.img)), 
	  delImg(std::move(t.delImg)),
	  texID(std::move(t.texID)), 
	  type(std::move(t.type))
{
	t.texID = 0;
}
void Texture::operator=(Texture&& t){
	img = std::move(t.img);
	
	texID = std::move(t.texID);
	t.texID = 0;
	
	type = std::move(t.type);
}

void Texture::init(bool isPixelated){
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
    
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (isPixelated ? GL_NEAREST : GL_LINEAR));
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (isPixelated ? GL_NEAREST : GL_LINEAR));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
	glDeleteTextures(1, &texID); 
	
	if(delImg)
		delete img; 
}

void Texture::Bind() const {
	if(this == currBoundTex)
		return;

    glActiveTexture(GL_TEXTURE0 + type);
    glBindTexture(GL_TEXTURE_2D, texID);
	currBoundTex = this;
}
