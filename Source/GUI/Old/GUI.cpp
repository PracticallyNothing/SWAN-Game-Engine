#include "../include/GUI.hpp"

#include <algorithm>
#include "Input.hpp"
#include "Util.hpp"

namespace GUI {
IElement* currFocused;
void setCurrentlyFocused(IElement* el) { currFocused = el; }
bool noCurrFocused() { return currFocused == NULL; }

// -----------------------------------------------------------
Image::Image(Texture* image) {
    img = image;
    fitToImage();
}
Image::Image(Texture* image, int w, int h) {
    assert(image != NULL);
    img = image;
    x = 0;
    y = 0;
    this->w = w;
    this->h = h;
}
void Image::fitToImage() {
    assert(img != NULL);
    w = img->getW();
    h = img->getH();
}
Texture* Image::getTexture() { return img; }
// -----------------------------------------------------------
Texture* Clickable::getTexture() {
    switch (currState) {
        case Clickable::State::INACTIVE:
            return inactive;

        case Clickable::State::ACTIVE:
            if (!active)
                return inactive;
            else
                return active;

        case Clickable::State::PRESSED:
            return pressed;

        default:
            return inactive;
    }
}

void Clickable::update() {
    bool mousedOver = Input.Mouse.x > x && Input.Mouse.y > y &&
                      Input.Mouse.x < x + w && Input.Mouse.y < y + h;
    bool focused = currFocused == this;
    bool noneFocused = noCurrFocused();

    if ((noneFocused || focused) &&
        (mousedOver || currState == State::PRESSED) && Input.Mouse.lButton) {
        currState = State::PRESSED;
        setCurrentlyFocused(this);
    } else if (mousedOver) {
        currState = State::ACTIVE;
    } else {
        currState = State::INACTIVE;
    }

    if (currState == State::INACTIVE && focused) {
        setCurrentlyFocused(NULL);
    }
}
// -----------------------------------------------------------
Renderer::Renderer() {
    shad = new Shader();
    shad->compileShaders("res/shaders/GUIShader.vs",
                         "res/shaders/GUIShader.fs");
    shad->addAttrib("pos");
    shad->addAttrib("UV");
    shad->addAttrib("norm");
    shad->linkShaders();

    shad->use();
    shad->addUniform("transform");
    shad->unuse();

    Vertex v1(glm::vec3(-1, -1, 0), glm::vec2(0, 0));
    Vertex v2(glm::vec3(1, 1, 0), glm::vec2(1, 1));
    Vertex v3(glm::vec3(1, -1, 0), glm::vec2(1, 0));
    Vertex v4(glm::vec3(-1, 1, 0), glm::vec2(0, 1));

    rect = new Mesh({v1, v2, v3, v4}, {0, 1, 2, 0, 3, 1});

    sortedByLayer = true;
}

Renderer::~Renderer() {
    for (Renderer::ElementType ei : elems) {
        if (ei.hasFirst()) {
            delete ei.getFirst();
        } else {
            for (IElement* e : ei.getSecond()->getElements().elements) {
                delete e;
            }
        }
    }

    delete rect;
    delete shad;
}

void Renderer::add(IElement* elem) {
    sortedByLayer = false;
    elems.push_back(Renderer::ElementType(elem));
}

void Renderer::add(IElementContainer* elemCont) {
    sortedByLayer = false;
    elems.push_back(Renderer::ElementType(elemCont));
}

void GUI::Renderer::renderElement(IElement* e, Display& d) {
    if (e->x > d.getW() || e->y > d.getH() || e->x + e->w < 0 ||
        e->y + e->h < 0)
        return;

    transform.getScale().x = (double)e->w / d.getW();
    transform.getScale().y = (double)e->h / d.getH();

    transform.getPos().x = Util::pixelToGLCoord(d.getW(), e->x + e->w / 2);
    transform.getPos().y =
        Util::pixelToGLCoord(d.getH(), d.getH() - (e->y + e->h / 2));

    shad->setUniformData("transform", transform);

    e->getTexture()->Bind();
    e->preRender();
    rect->render();
    e->postRender();
}

void GUI::Renderer::render(Display& d) {
    if (!sortedByLayer) {
        std::sort(elems.begin(), elems.end(), LayerSorter());
        sortedByLayer = true;
    }

    shad->use();

    for (GUI::Renderer::ElementType ei : elems) {
        if (ei.hasFirst()) {
            renderElement(ei.getFirst(), d);
        } else {
            ei.getSecond()->preGroupRender();
            for (IElement* e : ei.getSecond()->getElements().elements) {
                renderElement(e, d);
            }
            ei.getSecond()->postGroupRender();
        }
    }

    shad->unuse();
}

void Renderer::update() {
    for (auto ei : elems) {
        if (ei.hasFirst()) {
            ei.getFirst()->update();
        } else {
            for (IElement* e : ei.getSecond()->getElements().elements) {
                e->update();
            }
        }
    }
}
const std::vector<Renderer::ElementType>& Renderer::getElems() const {
    return elems;
}

Renderer::ElementType& Renderer::getElem(int index) { return elems[index]; }
// -----------------------------------------------------------
void Draggable::update() {
    bool mousedOver = Input.Mouse.x > x && Input.Mouse.y > y &&
                      Input.Mouse.x < x + w && Input.Mouse.y < y + h;

    if ((currFocused == this || noCurrFocused()) && mousedOver &&
        Input.Mouse.lButton) {
        setCurrentlyFocused(this);
        int relMouseX = Input.Mouse.x - x, relMouseY = Input.Mouse.y - y;
        if (lastX < 0) {
            lastX = relMouseX;
        }
        if (lastX < 0) {
            lastY = relMouseY;
        }

        x -= lastX - relMouseX;
        y -= lastY - relMouseY;

        lastX = relMouseX;
        lastY = relMouseY;
    } else {
        if (currFocused == this) {
            setCurrentlyFocused(NULL);
        }

        lastX = -1;
        lastY = -1;
    }
}
}
