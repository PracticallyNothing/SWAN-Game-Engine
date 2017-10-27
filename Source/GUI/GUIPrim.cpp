#include "GUIPrim.hpp"

#include <algorithm>

#include "Core/Input.hpp"
#include "Core/Resources.hpp"

#include "Utility/Math.hpp"

namespace SWAN {
namespace GUIP {
	IElement* currFocused;
	void setCurrentlyFocused(IElement* el) { currFocused = el; }
	bool noCurrFocused() { return currFocused == NULL; }

	// -----------------------------------------------------------
	Image::Image(const Texture* image) {
		img = image;
		fitToImage();
	}
	Image::Image(const Texture* image, int w, int h) : IElement(0, 0, w, h) {
		assert(image != NULL);
		img = image;
		x   = 0;
		y   = 0;
	}
	void Image::fitToImage() {
		assert(img != NULL);
		w = img->getW();
		h = img->getH();
	}
	const Texture* Image::getTexture() { return img; }
	// -----------------------------------------------------------
	const Texture* Button::getTexture() {
		switch(currState) {
			case Button::State::INACTIVE: return inactive;

			case Button::State::ACTIVE:
				if(!active)
					return inactive;
				else
					return active;

			case Button::State::PRESSED: return pressed;

			default: return inactive;
		}
	}

	void Button::update() {
		bool mousedOver = SWAN_Input.Mouse.x > x && SWAN_Input.Mouse.y > y &&
		                  SWAN_Input.Mouse.x < x + w &&
		                  SWAN_Input.Mouse.y < y + h;
		bool focused     = (currFocused == this);
		bool noneFocused = noCurrFocused();

		if((noneFocused || focused) &&
		   (mousedOver || currState == State::PRESSED) &&
		   SWAN_Input.Mouse.lButton) {
			currState = State::PRESSED;
			setCurrentlyFocused(this);
		} else if(noneFocused && mousedOver) {
			currState = State::ACTIVE;
		} else {
			currState = State::INACTIVE;
		}

		if(currState == State::INACTIVE && focused) {
			setCurrentlyFocused(NULL);
		}
	}
	// -----------------------------------------------------------
	void Draggable::update() {
		bool mousedOver = SWAN_Input.Mouse.x > x && SWAN_Input.Mouse.y > y &&
		                  SWAN_Input.Mouse.x < x + w &&
		                  SWAN_Input.Mouse.y < y + h;

		if(currFocused == this) { // If being dragged ...
			if(SWAN_Input.Mouse
			       .lButton) { // ... and the mouse is still being held
				// Set position to mouse.xy - offset.xy
				if(!_lockX) {
					if(minX >= 0 && maxX >= 0 && maxX > minX)
						x = SWAN::Util::Clamp(SWAN_Input.Mouse.x + offsetX,
						                      minX, maxX);
					else
						x = SWAN_Input.Mouse.x + offsetX;
				}

				if(!_lockY) {
					if(minY >= 0 && maxY >= 0 && maxY > minY)
						y = SWAN::Util::Clamp(SWAN_Input.Mouse.y + offsetY,
						                      minY, maxY);
					else
						y = SWAN_Input.Mouse.y + offsetY;
				}
			} else { // ... but LMB isn't being pressed anymore
				// Reset the offset
				offsetX = offsetY = 0;

				// Change currFocused object to not be you
				currFocused = nullptr;
			}
		} else if(noCurrFocused()) { // If not being dragged and there are no
			                         // other
			                         // active elements ...
			if(mousedOver &&
			   SWAN_Input.Mouse.lButton) { // ... and the mouse is on top
				                           // of the element with LMB
				                           // pressed
				// Change currFocused to indicate you are being dragged
				currFocused = this;

				// Set offset to position.xy - mouse.xy
				offsetX = x - SWAN_Input.Mouse.x;
				offsetY = y - SWAN_Input.Mouse.y;
			}
		}
	}
}
}
