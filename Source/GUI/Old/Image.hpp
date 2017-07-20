#ifndef GUI_IMAGE_HPP
#define GUI_IMAGE_HPP

#include "Interface.hpp"

namespace GUI {

struct Image : public IElement {
    Texture* img;

    Image(Texture* image);
    Image(Texture* image, int w, int h);
    virtual ~Image() {}
    virtual Texture* getTexture();
    void fitToImage();
};

struct AnimatedImage : public IElement {
    Texture* frames;
    int numFrames, currentFrame, framerate;

    virtual Texture* getTexture();
    void update();
};
}

#endif  // GUI_IMAGE_HPP
