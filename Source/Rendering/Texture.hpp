#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <GL/glew.h>
#include "Image.hpp"

enum {
    TEXTURE_DIFFUSE_MAP,
    TEXTURE_SPECULAR_MAP,
    TEXTURE_NORMAL_MAP,
    TEXTURE_REFLECTION_MAP
};

class Texture{
    public:
        Texture(const std::string& fileName,
                bool isPixelated = false,
                int type = TEXTURE_DIFFUSE_MAP);

		Texture(const Image& img,
				bool isPixelated = false,
				int type = TEXTURE_DIFFUSE_MAP);

		Texture(const Texture& tex) = delete;
		Texture& operator=(const Texture& tex) = delete;

		Texture(Texture&& t);
		void operator=(Texture&& tex);

        ~Texture();

        void bind() const;

        inline int getW() const { return (img ? img->width : -1); }
        inline int getH() const { return (img ? img->height: -1); }

		const Image* getImage() const { return img; }
    private:
		void init(bool isPixelated);

		const Image* img;
		bool delImg;

        GLuint texID;
        int type;

		static const Texture* currBoundTex;
};

#endif
