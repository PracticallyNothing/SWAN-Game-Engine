#ifndef GUI_CLICKABLE_HPP
#define GUI_CLICKABLE_HPP

#include "Interface.hpp"

namespace GUI {

struct Clickable : public IElement {
    Texture *inactive, *active, *pressed;

    enum class State { INACTIVE, ACTIVE, PRESSED };
    State currState = State::INACTIVE;

    Texture* getTexture();
    void update();
};
}

#endif
