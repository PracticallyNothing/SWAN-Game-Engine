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

		res->listeners.push_back(onMouseEnter);
		res->listeners.push_back(onMouseLeave);
		res->listeners.push_back(onMousePress);
		res->listeners.push_back(onMouseRelease);

		return std::unique_ptr<GUIElement>(res);
	}
}
