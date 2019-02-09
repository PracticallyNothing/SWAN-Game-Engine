#ifndef SWAN_TEXTURE_HPP
#define SWAN_TEXTURE_HPP

#include <glad/glad.h>
#include <string>

#include "Image.hpp"

namespace SWAN
{
	enum {
		TEXTURE_DIFFUSE_MAP,
		TEXTURE_SPECULAR_MAP,
		TEXTURE_NORMAL_MAP,
		TEXTURE_REFLECTION_MAP
	};

	class Texture
	{
	  public:
		/** 
		 * @brief Create a texture by loading it from a file using the stb_image library.
		 *
		 * @note With this method, the texture becomes the owner of the loaded image,
		 *       and the image will be deleted when the texture is.
		 *
		 * @param fileName Name of the file to load on the disk.
		 * @param isPixelated Whether the texture should be magnified 
		 *                    and minified by copying the nearest pixel (when true) 
		 *                    or by averaging neighbouring pixels (when false).
		 *                    This setting should be true for any low resolution textures,
		 *                    so that there isn't blurring when the size is changed.
		 * @param type Currently does nothing.
		 */
		Texture(const std::string& fileName,
		        bool isPixelated = false,
		        int type = TEXTURE_DIFFUSE_MAP);

		/** 
		 * @brief Create a texture by directly using an image.
		 *
		 * @param img Reference to target image to use.
		 * @param isPixelated Whether the texture should be magnified 
		 *                    and minified by copying the nearest pixel (when true) 
		 *                    or by averaging neighbouring pixels (when false).
		 *                    This setting should be true for any low resolution textures,
		 *                    so that there isn't blurring when the size is changed.
		 * @param type Currently does nothing.
		 */
		Texture(const Image& img,
		        bool isPixelated = false,
		        int type = TEXTURE_DIFFUSE_MAP);

		/// Deleted copy constructor
		Texture(const Texture& tex) = delete;
		/// Deleted copy operator
		Texture& operator=(const Texture& tex) = delete;

		/// Move constructor
		Texture(Texture&& t);
		/// Move operator
		void operator=(Texture&& tex);

		/// Destructor
		~Texture();

		/// Binds the texture for use in OpenGL
		void bind() const;

		/// Get the width of the original image that makes this texture.
		inline int getW() const { return (img ? img->width : -1); }
		/// Get the height of the original image that makes this texture.
		inline int getH() const { return (img ? img->height : -1); }

		/// Get the original image that makes this texture.
		const Image* getImage() const { return img; }

	  private:
		void init(bool isPixelated);

		const Image* img;
		bool delImg;

		GLuint texID;
		int type;

		static const Texture* currBoundTex;
	};
} // namespace SWAN

#endif
