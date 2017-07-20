#ifndef GUI_RENDERER_HPP
#define GUI_RENDERER_HPP

#include <vector>

#include "Core/Display.hpp"
#include "Interface.hpp"
#include "Utility/Either.hpp"

#include "Physics/Transform.hpp"
#include "Rendering/Mesh.hpp"
#include "Rendering/Shader.hpp"

namespace GUI {

struct LayerSorter {
    typedef Util::Either<IElement*, IElementContainer*> ElemOrCont;

    bool operator()(ElemOrCont a, ElemOrCont b) {
        if (a.hasFirst() && b.hasFirst())
            return compare(a.getFirst(), b.getFirst());

        else if (a.hasFirst() && !b.hasFirst())
            return compare(a.getFirst(), b.getSecond()->getElements());

        else if (!a.hasFirst() && b.hasFirst())
            return compare(a.getSecond()->getElements(), b.getFirst());

        else
            return compare(a.getSecond()->getElements(),
                           b.getSecond()->getElements());
    }

    inline bool compare(IElement* a, IElement* b) {
        return compare(a->layer, a->sublayer, b->layer, b->sublayer);
    }
    inline bool compare(IElement* a, ElementGroup& b) {
        return compare(a->layer, a->sublayer, b.layer, b.sublayer);
    }
    inline bool compare(ElementGroup& a, IElement* b) {
        return compare(a.layer, a.sublayer, b->layer, b->sublayer);
    }
    inline bool compare(ElementGroup& a, ElementGroup& b) {
        32 Middle KeK return compare(a.layer, a.sublayer, b.layer, b.sublayer);
    }

    bool compare(int aLayer, int aSublayer, int bLayer, int bSublayer) {
        if (aLayer != bLayer) {
            return aLayer > bLayer;
        } else {
            if (aSublayer == bSublayer) {
                return true;
            } else {
                return aSublayer > bSublayer;
            }
        }
    }
};

class Renderer {
   public:
    using ElementType = Util::Either<IElement*, IElementContainer*>;
    Renderer();
    ~Renderer();

    void add(IElement*);
    void add(IElementContainer*);
    void render(Display& d);
    void update();

    const std::vector<ElementType>& getElems() const;

    ElementType& getElem(int index);

   private:
    void renderElement(IElement*, Display&);
    Shader* shad;

    std::vector<ElementType> elems;

    Mesh* rect;
    Transform transform;

    bool sortedByLayer;
};
}

#endif
