#ifndef GUI_HPP
#define GUI_HPP

#include <vector>
#include <utility>

#include "Rendering/Mesh.hpp"
#include "Rendering/Texture.hpp"
#include "Core/Display.hpp"
#include "Rendering/Shader.hpp"
#include "Utility/Either.hpp"

namespace GUI {
    //---Interfaces-----------------------//
    struct IElement;
    struct ElementGroup;
    struct IElementContainer;
    //---Derived from IElement------------//
    struct Image;
    struct AnimatedImage;
    struct Clickable;
    struct Draggable;
    //---Derived from IElementContainer---//
    class Window;
    //---Other----------------------------//
    struct LayerSorter;
    class Renderer;
    //------------------------------------//

    struct IElement {
        int x,y,w,h;
        int layer = 0, sublayer = 0;

        virtual ~IElement(){}
        virtual Texture* getTexture() = 0;
        virtual void update(){}
        virtual void preRender(){}
        virtual void postRender(){}
    };

    //    typedef std::pair<int,int> LayerInfo;
    //    typedef std::vector<IElement*> ElementGroup_impl;
    //    typedef std::pair<ElementGroup_impl, LayerInfo> ElementGroup;

    struct ElementGroup{
        int layer, sublayer;
        std::vector<IElement*> elements;
    };

    struct IElementContainer {
        virtual ElementGroup& getElements() = 0;
        virtual void preGroupRender(){}
        virtual void postGroupRender(){}
    };

    struct Image : public IElement {
        Texture* img;

        Image(Texture* image);
        Image(Texture* image,
                int w, int h);
        virtual ~Image(){}
        virtual Texture* getTexture();
        void fitToImage();
    };

    struct AnimatedImage : public IElement {
        Texture *frames;
        int numFrames, currentFrame, framerate;

        virtual Texture* getTexture();
        void update();
    };

    struct Clickable : public IElement {
        Texture *inactive, *active, *pressed;

        enum class State { INACTIVE, ACTIVE, PRESSED };
        State currState = State::INACTIVE;

        Texture* getTexture();
        void update();
    };

    struct Draggable : public IElement {
        Image img;

        Draggable(Image img) : img(img){
            this->w = img.w;
            this->h = img.h;
        }

        inline Texture* getTexture(){ return img.getTexture(); }
        void update();
        private:
        int lastX = -1, lastY = -1;
    };

    struct LayerSorter {
        typedef Util::Either<IElement*, IElementContainer*> ElemOrCont;

        bool operator()(ElemOrCont a, ElemOrCont b){
            if(a.hasFirst() && b.hasFirst())
                return compare(a.getFirst(),
                        b.getFirst());

            else if(a.hasFirst() && !b.hasFirst())
                return compare(a.getFirst(),
                        b.getSecond()->getElements());

            else if(!a.hasFirst() && b.hasFirst())
                return compare(a.getSecond()->getElements(),
                        b.getFirst());

            else
                return compare(a.getSecond()->getElements(),
                        b.getSecond()->getElements());
        }

        inline bool compare(IElement* a, IElement* b){
            return compare(a->layer, a->sublayer,
                    b->layer, b->sublayer);
        }
        inline bool compare(IElement* a, ElementGroup& b){
            return compare(a->layer, a->sublayer,
                    b.layer, b.sublayer);
        }
        inline bool compare(ElementGroup& a, IElement* b){
            return compare(a.layer, a.sublayer,
                    b->layer, b->sublayer);
        }
        inline bool compare(ElementGroup& a, ElementGroup& b){
            return compare(a.layer, a.sublayer,
                    b.layer, b.sublayer);
        }

        bool compare(int aLayer, int aSublayer, int bLayer, int bSublayer){
            if(aLayer != bLayer) {
                return aLayer > bLayer;
            } else {
                if(aSublayer == bSublayer) {
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
            void renderElement(IElement*,Display&);
            Shader* shad;

            std::vector<ElementType> elems;

            Mesh* rect;
            Transform transform;

            bool sortedByLayer;
    };

    class Window : public IElement {
        public:
            Window(unsigned topMargin , unsigned bottomMargin,
                    unsigned leftMargin, unsigned rightMargin){
                uMargin = topMargin; dMargin = bottomMargin;
                lMargin = leftMargin; rMargin = rightMargin;
            }

            Window(unsigned horizontalMargin, unsigned verticalMargin){
                uMargin = verticalMargin; dMargin = verticalMargin;
                lMargin = horizontalMargin; rMargin = horizontalMargin;
            }

        private:
            std::vector<IElement*> elements;
            int layer, sublayer;
            Draggable *top;
            Clickable *minimize, *maximize, *close;
            unsigned uMargin, dMargin, lMargin, rMargin;
    };

    extern IElement* currFocused;
    extern void setCurrentlyFocused(IElement* el);
    extern bool noCurrFocused();
}

#endif
