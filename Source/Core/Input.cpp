#include "Input.hpp"
#include <SDL2/SDL.h>

#ifndef __cplusplus
#define true 1
#define false 0
#endif // __cplusplus

_input SWAN_Input;

namespace SWAN {
namespace detail {
	_input prevState;
}

MouseState GetCurrMouseState() {
	return { SWAN_Input.Mouse.x, SWAN_Input.Mouse.y,

		     SWAN_Input.Mouse.lButton, SWAN_Input.Mouse.mButton,
		     SWAN_Input.Mouse.rButton };
}

MouseState GetPrevMouseState() {
	return { detail::prevState.Mouse.x, detail::prevState.Mouse.y,

		     detail::prevState.Mouse.lButton, detail::prevState.Mouse.mButton,
		     detail::prevState.Mouse.rButton };
}
}

void _handleEvents() {
	SWAN::detail::prevState = SWAN_Input;

	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		switch(e.type) {
			case SDL_QUIT: SWAN_Input.Window.exitRequest = true; break;
			case SDL_KEYDOWN: {
				unsigned int key = e.key.keysym.sym;

				if(key >= SDLK_0 && key <= SDLK_9) {
					SWAN_Input.Keyboard.numberKeys[key - SDLK_0] = true;
				} else if(key >= SDLK_a && key <= SDLK_z) {
					SWAN_Input.Keyboard.letterKeys[key - SDLK_a] = true;
				} else if(key >= SDLK_F1 && key <= SDLK_F12) {
					SWAN_Input.Keyboard.FKeys[key - SDLK_F1] = true;
				} else if(key >= SDLK_KP_0 && key <= SDLK_KP_9) {
					SWAN_Input.Keyboard.numpadKeys[key - SDLK_KP_0] = true;
				}

				else if(key == SDLK_ESCAPE) {
					SWAN_Input.Keyboard.escapeKey = true;
				} else if(key == SDLK_RETURN) {
					SWAN_Input.Keyboard.enterKey = true;
				} else if(key == SDLK_SPACE) {
					SWAN_Input.Keyboard.spaceKey = true;
				}

				else if(key == SDLK_LSHIFT) {
					SWAN_Input.Keyboard.LShiftKey = true;
				} else if(key == SDLK_RSHIFT) {
					SWAN_Input.Keyboard.RShiftKey = true;
				} else if(key == SDLK_LCTRL) {
					SWAN_Input.Keyboard.LCtrlKey = true;
				} else if(key == SDLK_RCTRL) {
					SWAN_Input.Keyboard.RCtrlKey = true;
				} else if(key == SDLK_LALT) {
					SWAN_Input.Keyboard.LAltKey = true;
				} else if(key == SDLK_RALT) {
					SWAN_Input.Keyboard.RAltKey = true;
				}

				else if(key == SDLK_RIGHT) {
					SWAN_Input.Keyboard.RArrowKey = true;
				} else if(key == SDLK_LEFT) {
					SWAN_Input.Keyboard.LArrowKey = true;
				} else if(key == SDLK_UP) {
					SWAN_Input.Keyboard.UArrowKey = true;
				} else if(key == SDLK_DOWN) {
					SWAN_Input.Keyboard.DArrowKey = true;
				}
			} break;
			case SDL_KEYUP: {
				unsigned int key = e.key.keysym.sym;

				if(key >= SDLK_0 && key <= SDLK_9) {
					SWAN_Input.Keyboard.numberKeys[key - SDLK_0] = false;
				} else if(key >= SDLK_a && key <= SDLK_z) {
					SWAN_Input.Keyboard.letterKeys[key - SDLK_a] = false;
				} else if(key >= SDLK_F1 && key <= SDLK_F12) {
					SWAN_Input.Keyboard.FKeys[key - SDLK_F1] = false;
				} else if(key >= SDLK_KP_0 && key <= SDLK_KP_9) {
					SWAN_Input.Keyboard.numpadKeys[key - SDLK_KP_0] = false;
				}

				else if(key == SDLK_ESCAPE) {
					SWAN_Input.Keyboard.escapeKey = false;
				} else if(key == SDLK_RETURN) {
					SWAN_Input.Keyboard.enterKey = false;
				} else if(key == SDLK_SPACE) {
					SWAN_Input.Keyboard.spaceKey = false;
				}

				else if(key == SDLK_LSHIFT) {
					SWAN_Input.Keyboard.LShiftKey = false;
				} else if(key == SDLK_RSHIFT) {
					SWAN_Input.Keyboard.RShiftKey = false;
				} else if(key == SDLK_LCTRL) {
					SWAN_Input.Keyboard.LCtrlKey = false;
				} else if(key == SDLK_RCTRL) {
					SWAN_Input.Keyboard.RCtrlKey = false;
				} else if(key == SDLK_LALT) {
					SWAN_Input.Keyboard.LAltKey = false;
				} else if(key == SDLK_RALT) {
					SWAN_Input.Keyboard.RAltKey = false;
				}

				else if(key == SDLK_RIGHT) {
					SWAN_Input.Keyboard.RArrowKey = false;
				} else if(key == SDLK_LEFT) {
					SWAN_Input.Keyboard.LArrowKey = false;
				} else if(key == SDLK_UP) {
					SWAN_Input.Keyboard.UArrowKey = false;
				} else if(key == SDLK_DOWN) {
					SWAN_Input.Keyboard.DArrowKey = false;
				}
			} break;
			case SDL_MOUSEMOTION:
				SWAN_Input.Mouse.x = e.motion.x;
				SWAN_Input.Mouse.y = e.motion.y;

				SWAN_Input.Mouse.xRel = e.motion.xrel;
				SWAN_Input.Mouse.yRel = e.motion.yrel;

				break;

			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button == SDL_BUTTON_LEFT) {
					SWAN_Input.Mouse.lButton = true;
				} else if(e.button.button == SDL_BUTTON_RIGHT) {
					SWAN_Input.Mouse.rButton = true;
				} else if(e.button.button == SDL_BUTTON_MIDDLE) {
					SWAN_Input.Mouse.mButton = true;
				}

				break;

			case SDL_MOUSEBUTTONUP:
				if(e.button.button == SDL_BUTTON_LEFT) {
					SWAN_Input.Mouse.lButton = false;
				} else if(e.button.button == SDL_BUTTON_RIGHT) {
					SWAN_Input.Mouse.rButton = false;
				} else if(e.button.button == SDL_BUTTON_MIDDLE) {
					SWAN_Input.Mouse.mButton = false;
				}

				break;
		}
	}
}

void SWAN_Input_Init() { SWAN_Input.handleEvents = _handleEvents; }
