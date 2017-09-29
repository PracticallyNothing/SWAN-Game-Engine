#ifndef GUI_HPP
#define GUI_HPP

#include <algorithm> // For std::max()
#include <vector>    // For std::vector<T>

#include "../Rendering/Texture.hpp" // For Texture

namespace GUIPrim {
	//---Interfaces-----------------------//
	struct IElement;
	struct ElementGroup;
	struct IElementContainer;
	//---Derived from IElement------------//
	struct Image;
	struct AnimatedImage;
	struct Button;
	struct Draggable;
	//---Derived from IElementContainer---//
	class Window;
	//------------------------------------//

	struct IElement {
		int x, y, w, h;
		int layer = 0, sublayer = 0;

		IElement(int x = 0, int y = 0, int w = 0, int h = 0, int layer = 0, int sublayer = 0)
			: x(x), y(y), w(w), h(h), layer(layer), sublayer(sublayer){}
		virtual ~IElement(){}
		virtual const Texture* getTexture() = 0;
		virtual void update(){}
		virtual void preRender(){}
		virtual void postRender(){}
	};

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
		const Texture* img;

		Image(const Texture* image);
		Image(const Texture* image, int w, int h);

		virtual ~Image(){}
		virtual const Texture* getTexture();

		void fitToImage();
	};

	struct AnimatedImage : public IElement {
		std::vector<const Texture *> frames;
		int currentFrame, framerate;

		virtual const Texture* getTexture();
		void update();
	};

	struct Button : public IElement {
		Button(const Texture* inactive,
				const Texture* active,
				const Texture* pressed)
			: inactive(inactive), active(active), pressed(pressed)
		{
			w = std::max(std::max(inactive->getW(), active->getW()), pressed->getW());
			h = std::max(std::max(inactive->getH(), active->getH()), pressed->getH());
		}

		Button(const Texture* inactive,
				const Texture* active,
				const Texture* pressed,
				int w, int h)
			: IElement(0, 0, w, h), inactive(inactive), active(active), pressed(pressed) {}

		const Texture *inactive, *active, *pressed;

		enum class State { INACTIVE, ACTIVE, PRESSED };
		State currState = State::INACTIVE;

		const Texture* getTexture();
		void update();
	};

	struct Draggable : public IElement {
		public:
			Draggable(const Texture* tex)
				: IElement(0, 0, tex->getW(), tex->getH()), texture(tex) {}

			Draggable(const Texture* tex, int w, int h)
				: IElement(0, 0, w, h), texture(tex) {}

			void update();

			const Texture* getTexture(){ return texture; }

			const Texture* texture;
		private:
			int offsetX = 0, offsetY = 0;
			bool moving = false;
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
			Draggable *top;
			Button *minimize, *maximize, *close;
			unsigned uMargin, dMargin, lMargin, rMargin;
	};

	extern IElement* currFocused;
	extern void setCurrentlyFocused(IElement* el);
	extern bool noCurrFocused();
}

#endif
