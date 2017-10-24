#include "GUIElement.hpp"

namespace SWAN {
	const GUIElement* GUIElement::focused = nullptr;
	
	std::unique_ptr<GUIElement> CreateImage (
		int x, int y,
		int w, int h,
		const Texture* texture
	){
		GUIElement* res = new GUIElement(x, y, w, h, GUIElement::T_TEXTURE);
		res->renderData.texture = texture;

		EventListener onMouseEnter (
			[el = res]() -> bool {
				return GUIElement::focused == nullptr
					&& GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h)
					&& !GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
			},
			[el = res] {
				GUIElement::focused = el;
			}
		);
		
		onMouseEnter.type = "mouse";
		onMouseEnter.description = "Activates when the mouse enters the area of the GUI element.";

		EventListener onMouseLeave (
			[el = res]() -> bool {
				return GUIElement::focused == el
					&& !GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h)
					&& GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
			},
			[el = res] {
				GUIElement::focused = nullptr;
			}
		);

		onMouseLeave.type = "mouse";
		onMouseLeave.description = "Activates when the mouse leaves the area of the GUI element.";

		res->listeners.push_back(onMouseEnter);
		res->listeners.push_back(onMouseLeave);

		return std::unique_ptr<GUIElement>(res);
	}

	std::unique_ptr<GUIElement> CreateButton (
		int x, int y,
		int w, int h,
		const Texture* normal,
		const Texture* focused,
		const Texture* active,
		EventListener::ActionFuncT onRelease
	) {
		GUIElement* res = new GUIElement(x, y, w, h, GUIElement::T_TEXTURE);

		res->renderData.texture = normal;

		EventListener onMouseEnter (
			[el = res]() -> bool {
				return GUIElement::focused == nullptr
					&& GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h)
					&& !GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
			},
			[el = res, focused] {
				el->renderData.texture = focused;
				GUIElement::focused = el;
			}
		);
		onMouseEnter.type = "mouse";
		onMouseEnter.description = "Activates when the mouse enters the area of the GUI element.";

		EventListener onMouseLeave (
			[el = res]() -> bool {
				return GUIElement::focused == el
					&& !GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h)
					&& GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
			},
			[el = res, normal] {
				el->renderData.texture = normal;
				GUIElement::focused = nullptr;
			}
		);
		onMouseLeave.type = "mouse";
		onMouseLeave.description = "Activates when the mouse leaves the area of the GUI element.";

		EventListener onMousePress (
			[el = res]() -> bool {
				return GUIElement::focused == el
					&& GetCurrMouseState().LMB
					&& !GetPrevMouseState().LMB;
			},
			[el = res, active] {
				el->renderData.texture = active;
			}
		);
		onMousePress.type = "mouse";
		onMousePress.description = "Activates when the LMB is pressed over a focused GUI element.";

		EventListener onMouseRelease (
			[el = res]() -> bool {
				return GUIElement::focused == el
					&& !GetCurrMouseState().LMB
					&& GetPrevMouseState().LMB;
			},
			[el = res, normal, onRelease] {
				el->renderData.texture = active;
				onRelease();
			}
		);
		onMouseRelease.type = "mouse";
		onMouseRelease.description = "Activates when the LMB is released over a focused GUI element.";

		res->listeners.push_back(onMouseEnter);
		res->listeners.push_back(onMouseLeave);
		res->listeners.push_back(onMousePress);
		res->listeners.push_back(onMouseRelease);

		return std::unique_ptr<GUIElement>(res);
	}
}
