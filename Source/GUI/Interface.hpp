#ifndef GUI_INTERFACE_HPP
#define GUI_INTERFACE_HPP

#include "Rendering/Texture.hpp"

namespace GUI {

struct IElement {
    int x, y, w, h;
    int layer = 0, sublayer = 0;

    virtual ~IElement() {}
    virtual Texture* getTexture() = 0;
    virtual void update() {}
    virtual void preRender() {}
    virtual void postRender() {}
};

struct ElementGroup {
    int layer, sublayer;
    std::vector<IElement*> elements;
};

struct IElementContainer {
    virtual ElementGroup& getElements() = 0;
    virtual void preGroupRender() {}
    virtual void postGroupRender() {}
};
}

#endif  // GUI_INTERFACE_HPP
