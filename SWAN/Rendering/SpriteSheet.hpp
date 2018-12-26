#ifndef SWAN_SPRITESHEET_HPP
#define SWAN_SPRITESHEET_HPP

#include "../Core/Defs.hpp"

#include "Texture.hpp"
#include "../Maths/Vector.hpp"

#include <map> // For std::map<K,V>

namespace SWAN {
    struct Sprite {
	ivec2 dimensions;
	fvec2 uvMin, uvMax;
	const Texture* source;

	/**
	 *  @brief Render the sprite with an overriden color and alpha.
	 *
	 *  @param pos Onscreen position to render the sprite to.
	 *
	 *  @param dim Dimensions to render the sprite with. 
	 *             If a dimension is less than 0, the size will be the same as the original image's size.
	 *             Default is {-1,-1}.
	 */
	void render(ivec2 pos, ivec2 dim = {-1,-1}) const; 

	/**
	 *  @brief Render the sprite with an overriden color and alpha.
	 *
	 *  @param pos Onscreen position to render the sprite to.
	 *
	 *  @param dim Dimensions to render the sprite with. 
	 *             If a dimension is less than 0, the size will be the same as the original image's size.
	 *
	 *  @param color What new color to use for rendering the sprite.
	 *
	 *  @param alpha What new alpha to use for rendering the sprite. 
	 *               If less than 0, image's original aplha is used.
	 *               Default is -1;
	 */
	void renderOverride(ivec2 pos, ivec2 dim, vec3 color, double alpha = -1) const; 
    };

    struct SpriteSheet : public Vector<Sprite> {
	/**
	 * @brief Generate sprites for a given texture and sprite size.
	 *
	 *
	 */
	void genSprites(const Texture* source, unsigned spriteWidth, unsigned spriteHeight) {
	    if(spriteWidth == 0 || spriteHeight == 0 || source == nullptr)
		return;

	    // Transform pixel coordinates to OpenGL UV coordinates.
	    auto XToU = [&source](int x) -> float { return float(x) / source->getW(); };
	    auto YToV = [&source](int y) -> float { return float(y) / source->getH(); };
	    //auto YToV = [&source](int y) -> float { return 1.0 + float(-y) / source->getH(); };

	    for(int y = 0; y < source->getH(); y += spriteHeight) {
		for(int x = 0; x < source->getW(); x += spriteWidth) {
		    push_back({ivec2(spriteWidth, spriteHeight),
			       fvec2(XToU(x), YToV(y)),
			       fvec2(XToU(x + spriteWidth), YToV(y + spriteHeight)),
			       source});
		}
	    }
	}
    };
}

#endif
