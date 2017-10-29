#include "GUIElement.hpp"

#include "Utility/Debug.hpp" // For SWAN_DEBUG_PRINT()

#include <algorithm> // For std::max(), std::min()
#include <cassert>

namespace SWAN {
const GUIElement* GUIElement::focused = nullptr;

static EventListener CreateOnMouseEnterListener(
    GUIElement* el,
    EventListener::ActionFuncT action) {
	EventListener res(
	    [el]() -> bool {
		    return GUIElement::focused == nullptr &&
		           GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h) &&
		           !GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
		},
	    [el, action] {
		    GUIElement::focused = el;
		    action();
		});

	res.type        = "mouse";
	res.description = "Activates when the mouse enters the area of the GUI element.";

	return res;
}

static EventListener CreateOnMouseLeaveListener(
    GUIElement* el,
    EventListener::ActionFuncT action) {
	EventListener res(
	    [el]() -> bool {
		    return el->isFocused() &&
		           !GetCurrMouseState().mousesOver(el->x, el->y, el->w, el->h) &&
		           GetPrevMouseState().mousesOver(el->x, el->y, el->w, el->h);
		},
	    [action] {
		    GUIElement::focused = nullptr;
		    action();
		});

	res.type        = "mouse";
	res.description = "Activates when the mouse leaves the area of the GUI element.";

	return res;
}

static EventListener CreateOnMousePressListener(
    GUIElement* el,
    EventListener::ActionFuncT action) {

	EventListener res(
	    [el]() -> bool {
		    return el->isFocused() && GetCurrMouseState().LMB;
		},
	    action);

	res.type        = "mouse";
	res.description = "Activates when the LMB is pressed over a focused GUI element.";

	return res;
}

static EventListener CreateOnMouseReleaseListener(
    GUIElement* el,
    EventListener::ActionFuncT action) {

	EventListener res(
	    [el]() -> bool {
		    return el->isFocused() &&
		           !GetCurrMouseState().LMB &&
		           GetPrevMouseState().LMB;
		},
	    action);

	res.type        = "mouse";
	res.description = "Activates when the LMB is released over a focused GUI element.";

	return res;
}

static inline EventListener CreateOnMouseEnterListener(GUIElement* el) {
	return CreateOnMouseEnterListener(el, [] {});
}
static inline EventListener CreateOnMouseLeaveListener(GUIElement* el) {
	return CreateOnMouseLeaveListener(el, [] {});
}

std::unique_ptr<GUIElement> CreateImage(
    int x, int y,
    int w, int h,
    const Texture* texture) {
	GUIElement* res = new GUIElement(x, y, w, h, GUIElement::T_TEXTURE);

	res->renderData.texture = texture;

	res->listeners.push_back(CreateOnMouseEnterListener(res));
	res->listeners.push_back(CreateOnMouseLeaveListener(res));

	return std::unique_ptr<GUIElement>(res);
}

std::unique_ptr<GUIElement> CreateButton(
    int x, int y,
    int w, int h,
    const Texture* normal,
    const Texture* focused,
    const Texture* active,
    EventListener::ActionFuncT onRelease) {

	GUIElement* res = new GUIElement(x, y, w, h, GUIElement::T_TEXTURE);

	assert(normal);
	assert(focused);
	assert(active);

	res->renderData.texture = normal;

	res->listeners.push_back(CreateOnMouseEnterListener(res, [res, focused] { res->renderData.texture = focused; }));
	res->listeners.push_back(CreateOnMouseLeaveListener(res, [res, normal] { res->renderData.texture = normal; }));
	res->listeners.push_back(CreateOnMousePressListener(res, [res, active] { res->renderData.texture = active; }));
	res->listeners.push_back(CreateOnMouseReleaseListener(res, [res, normal, onRelease] {
		res->renderData.texture = normal;
		onRelease();
	}));

	return std::unique_ptr<GUIElement>(res);
}

std::unique_ptr<GUIElement> CreateSlider(
    int x, int y,
    int w, int h,
    Color bgColor,
    Color handleColor,
    Color activeColor,
    std::function<void(double)> onChange) {

	GUIElement* bg = new GUIElement(x, y, w, h, GUIElement::T_COLOR);

	GUIElement* handle = new GUIElement(
	    (int) x + (w * 0.9) / 2,
	    (int) y + (h * 0.2) / 2,
	    (int) w * 0.1,
	    (int) h * 0.8,
	    GUIElement::T_COLOR);

	GUIElement* active = new GUIElement(x, y, w / 2, h, GUIElement::T_COLOR);

	bg->renderData.color     = bgColor;
	handle->renderData.color = handleColor;
	active->renderData.color = activeColor;

	// Note: The handle has no event listeners.
	//       Same goes for the active part.
	//       That's because they are just cosmetics.
	//       In reality, the background is tracking all the changes.

	bg->listeners.push_back(CreateOnMouseEnterListener(bg));
	bg->listeners.push_back(CreateOnMouseLeaveListener(bg));
	bg->listeners.push_back(CreateOnMousePressListener(
	    bg,
	    [bg, handle, active, onChange] {
		    active->w = GetCurrMouseState().x - bg->x;
		    handle->x = bg->x + active->w - handle->w / 2;
		    onChange((double) active->w / bg->w);
		}));

	bg->children.emplace_back(active);
	bg->children.emplace_back(handle);

	return std::unique_ptr<GUIElement>(bg);
}
}
