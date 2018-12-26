#include "Event.hpp"

#include <SDL2/SDL.h>
#include <list>
#include <vector>

#include "InputFrame.hpp"
#include "FlatInputRep.hpp"

#include "Core/Logging.hpp"

namespace SWAN
{
    static std::vector<Event> EventQueue;
    static std::list<KeyCode> heldKeys;

#define CONV(x) KeyCode(x)
    static KeyCode SDLKeyToSWANKey(unsigned SDLKey)
    {
	KeyCode k;

	if(SDLKey >= SDLK_a && SDLKey <= SDLK_z) {
	    return CONV(A + (SDLKey - SDLK_a));
	}

	if(SDLKey >= SDLK_0 && SDLKey <= SDLK_9) {
	    if(SDLKey == SDLK_0)
		return K0;
	    return CONV(K1 + (SDLKey - SDLK_1));
	}

	if(SDLKey >= SDLK_F1 && SDLKey <= SDLK_F12)
	    return CONV(F1 + (SDLKey - SDLK_F1));

	if(SDLKey >= SDLK_KP_0 && SDLKey <= SDLK_KP_9)
	    return CONV(N0 + (SDLKey - SDLK_KP_0));

	switch(SDLKey)
	{
	case SDLK_ESCAPE: return Escape;
	case SDLK_BACKQUOTE: return Grave;
	case SDLK_TAB: return Tab;
	case SDLK_CAPSLOCK: return CapsLock;
	case SDLK_LSHIFT: return LShift;
	case SDLK_LCTRL: return LCtrl;
	case SDLK_LALT: return LAlt;
	case SDLK_SPACE: return Spacebar;

	case SDLK_MINUS: return Minus;
	case SDLK_EQUALS: return Equals;

	case SDLK_BACKSPACE: return Backspace;
	case SDLK_RETURN: return Enter;
	case SDLK_RSHIFT: return RShift;
	case SDLK_RCTRL: return RCtrl;
	case SDLK_RALT: return RAlt;

	case SDLK_COMMA: return Comma;
	case SDLK_PERIOD: return Period;
	case SDLK_SLASH: return Slash;
	case SDLK_SEMICOLON: return Semicolon;
	case SDLK_QUOTE: return Apostrophe;
	case SDLK_BACKSLASH: return Backslash;
	case SDLK_LEFTBRACKET: return LBracket;
	case SDLK_RIGHTBRACKET: return RBracket;

	case SDLK_INSERT: return Insert;
	case SDLK_DELETE: return Delete;
	case SDLK_HOME: return Home;
	case SDLK_END: return End;
	case SDLK_PAGEUP: return PageUp;
	case SDLK_PAGEDOWN: return PageDown;

	case SDLK_UP: return UpArrow;
	case SDLK_DOWN: return DownArrow;
	case SDLK_LEFT: return LeftArrow;
	case SDLK_RIGHT: return RightArrow;
	}

	return TotalKeys;
    }
#undef CONV

    static Event WindowResizeEvent(unsigned width, unsigned height) {
	Event res(EventType::WindowResize);
	res.WindowResizeData = WindowResize(width, height);
	return res;
    }

    static void KeySetMods(Key& key, SDL_Keymod mod = SDL_GetModState()) {
	key.LShiftHeld = mod & KMOD_LSHIFT;
	key.RShiftHeld = mod & KMOD_RSHIFT;
	key.LCtrlHeld  = mod & KMOD_LCTRL;
	key.RCtrlHeld  = mod & KMOD_RCTRL;
	key.LAltHeld   = mod & KMOD_LALT;
	key.RAltHeld   = mod & KMOD_RALT;
    }

    static Event KeyPressedEvent(SDL_KeyboardEvent ke)
    {
	Event res;
	res.KeyData.Code = SDLKeyToSWANKey(ke.keysym.sym);

	if(detail::flatInput.Keys[res.KeyData.Code])
	   res.Type = EventType::KeyRepeat;
	else {
	   res.Type = EventType::KeyPress;
	   heldKeys.push_back(res.KeyData.Code);
	}

	KeySetMods(res.KeyData);

	detail::flatInput.Keys[res.KeyData.Code] = true;
	
	return res;
    }

    static Event KeyHoldEvent(KeyCode kc)
    {
	Event res(EventType::KeyHold);
	res.KeyData.Code = kc;
	KeySetMods(res.KeyData);
	return res;
    }

    static Event KeyReleasedEvent(SDL_KeyboardEvent ke)
    {
	Event res(EventType::KeyRelease);
	res.KeyData.Code = SDLKeyToSWANKey(ke.keysym.sym);
	KeySetMods(res.KeyData);

	heldKeys.remove(res.KeyData.Code);
	detail::flatInput.Keys[res.KeyData.Code] = false;

	return res;
    }

    static Event MouseMoveEvent(SDL_MouseMotionEvent mme)
    {
	Event res(EventType::MouseMove);
	res.MouseMoveData.X = mme.x;
	res.MouseMoveData.Y = mme.y;
	res.MouseMoveData.RelativeX = mme.xrel;
	res.MouseMoveData.RelativeY = mme.yrel;

	if(MouseWarps.front() == res.MouseMoveData)
	{
	    MouseWarps.pop_front();
	    return Event();
	}

	detail::flatInput.MouseState.X = mme.x;
	detail::flatInput.MouseState.Y = mme.y;
	
	return res;
    }

    static MouseButton SDLButtonToSWANMouseKey(uint8_t button)
    {
	if(button == SDL_BUTTON_LEFT)   return MouseButton::LMB;
	else if(button == SDL_BUTTON_MIDDLE) return MouseButton::MMB;
	else if(button == SDL_BUTTON_RIGHT)  return MouseButton::RMB;
    }

    static Event MouseKeyPressEvent(SDL_MouseButtonEvent mbe)
    {
	Event res(EventType::MouseKeyPress);
	res.MouseButtonData.Button = SDLButtonToSWANMouseKey(mbe.button);

	switch(res.MouseButtonData.Button)
	{
	case MouseButton::LMB: detail::flatInput.MouseState.LMB = true; break;
	case MouseButton::MMB: detail::flatInput.MouseState.MMB = true; break;
	case MouseButton::RMB: detail::flatInput.MouseState.RMB = true; break;
	}

	res.MouseButtonData.X = GetFlatRep().MouseState.X;
	res.MouseButtonData.Y = GetFlatRep().MouseState.Y;

	return res;
    }

    static auto MouseKeyReleaseEvent(SDL_MouseButtonEvent mbe)
    {
	Event res(EventType::MouseKeyRelease);

	res.MouseButtonData.Button = SDLButtonToSWANMouseKey(mbe.button);

	switch(res.MouseButtonData.Button)
	{
	case MouseButton::LMB: detail::flatInput.MouseState.LMB = false; break;
	case MouseButton::MMB: detail::flatInput.MouseState.MMB = false; break;
	case MouseButton::RMB: detail::flatInput.MouseState.RMB = false; break;
	}

	res.MouseButtonData.X = GetFlatRep().MouseState.X;
	res.MouseButtonData.Y = GetFlatRep().MouseState.Y;

	return res;
    }

    static auto MouseScrollEvent(SDL_MouseWheelEvent mwe)
    {
	Event res(EventType::MouseScroll);
	res.MouseScrollData.Amount = mwe.y;
	detail::flatInput.MouseState.ScrollAmount = mwe.y;
	return res;
    }

    void UpdateInputEvents()
    {
	GetPrevFlatRep() = GetFlatRep();

	int numMouseMoves = 0;

	SDL_Event e;
	while(SDL_PollEvent(&e)) {
	    switch(e.type) {
	    case SDL_KEYDOWN: EventQueue.push_back(KeyPressedEvent(e.key)); break;
	    case SDL_KEYUP:   EventQueue.push_back(KeyReleasedEvent(e.key)); break;

	    case SDL_MOUSEBUTTONDOWN: EventQueue.push_back(MouseKeyPressEvent(e.button)); break;
	    case SDL_MOUSEBUTTONUP:   EventQueue.push_back(MouseKeyReleaseEvent(e.button)); break;
	    case SDL_MOUSEMOTION:     EventQueue.push_back(MouseMoveEvent(e.motion)); numMouseMoves++; break;
	    case SDL_MOUSEWHEEL:      EventQueue.push_back(MouseScrollEvent(e.wheel)); break;

	    case SDL_QUIT:                     EventQueue.push_back(Event(EventType::WindowExit)); break;
	    case SDL_WINDOWEVENT_FOCUS_LOST:   EventQueue.push_back(Event(EventType::WindowLoseFocus)); break;
	    case SDL_WINDOWEVENT_FOCUS_GAINED: EventQueue.push_back(Event(EventType::WindowRegainFocus)); break;
	    case SDL_WINDOWEVENT_LEAVE:        EventQueue.push_back(Event(EventType::WindowMouseLeave)); break;
	    case SDL_WINDOWEVENT_ENTER:        EventQueue.push_back(Event(EventType::WindowMouseEnter)); break;
	    case SDL_WINDOWEVENT_RESIZED:      EventQueue.push_back(WindowResizeEvent(e.window.data1, e.window.data2)); break;
	    }
	}

	if(numMouseMoves > 1) LogVar<int>("numMouseMoves", numMouseMoves);

	for(KeyCode kc : heldKeys)
	    EventQueue.push_back(KeyHoldEvent(kc));

	for(Event e : EventQueue)
	{
	    InputFrame* frame = GetCurrentInputFrame();
	    if(frame == nullptr)
		return;

	    switch(e.Type)
	    {
	    case EventType::None: break;

	    case EventType::KeyPress:   frame->OnKeyPress(e.KeyData); break;
	    case EventType::KeyRepeat:  frame->OnKeyRepeat(e.KeyData); break;
	    case EventType::KeyHold:    frame->OnKeyHold(e.KeyData); break;
	    case EventType::KeyRelease: frame->OnKeyRelease(e.KeyData); break;

	    case EventType::MouseKeyPress:   frame->OnMouseKeyPress(e.MouseButtonData); break;
	    case EventType::MouseKeyRelease: frame->OnMouseKeyRelease(e.MouseButtonData); break;
	    case EventType::MouseMove:       frame->OnMouseMove(e.MouseMoveData); break;
	    case EventType::MouseScroll:     frame->OnMouseScroll(e.MouseScrollData); break;

	    case EventType::WindowExit:        frame->OnWindowExit(); break;
	    case EventType::WindowResize:      frame->OnWindowResize(e.WindowResizeData); break;
	    case EventType::WindowLoseFocus:   frame->OnWindowLoseKeyboardFocus(); break;
	    case EventType::WindowRegainFocus: frame->OnWindowRegainKeyboardFocus(); break;
	    case EventType::WindowMouseLeave:  frame->OnWindowMouseLeave(); break;
	    case EventType::WindowMouseEnter:  frame->OnWindowMouseEnter(); break;
	    }
	}

	EventQueue.clear();
    }
}
