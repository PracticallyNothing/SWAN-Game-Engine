#ifndef GUI_DRAGGABLE_HPP
#define GUI_DRAGGABLE_HPP

#include "Interface.hpp"

namespace GUI {

struct Draggable : public IElement {
    Image img;

    Draggable(Image img) : img(img) {
        this->w = img.w;
        this->h = img.h;
    }

    inline Texture* getTexture() { return img.getTexture(); }
    void update();

   private:
    int lastX = -1, lastY = -1;
};
}

#endif
