#include <SDL2/SDL.h>
#include "Input.hpp"

#ifndef __cplusplus
#define true 1
#define false 0
#endif  // __cplusplus

_input Input;

void _handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                Input.Window.exitRequest = true;
                break;
            case SDL_KEYDOWN: {
                unsigned int key = e.key.keysym.sym;

                if (key >= SDLK_0 && key <= SDLK_9) {
                    Input.Keyboard.numberKeys[key - SDLK_0] = true;
                } else if (key >= SDLK_a && key <= SDLK_z) {
                    Input.Keyboard.letterKeys[key - SDLK_a] = true;
                } else if (key >= SDLK_F1 && key <= SDLK_F12) {
                    Input.Keyboard.FKeys[key - SDLK_F1] = true;
                } else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                    Input.Keyboard.numpadKeys[key - SDLK_KP_0] = true;
                }

                else if (key == SDLK_ESCAPE) {
                    Input.Keyboard.escapeKey = true;
                } else if (key == SDLK_RETURN) {
                    Input.Keyboard.enterKey = true;
                } else if (key == SDLK_SPACE) {
                    Input.Keyboard.spaceKey = true;
                }

                else if (key == SDLK_LSHIFT) {
                    Input.Keyboard.LShiftKey = true;
                } else if (key == SDLK_RSHIFT) {
                    Input.Keyboard.RShiftKey = true;
                } else if (key == SDLK_LCTRL) {
                    Input.Keyboard.LCtrlKey = true;
                } else if (key == SDLK_RCTRL) {
                    Input.Keyboard.RCtrlKey = true;
                } else if (key == SDLK_LALT) {
                    Input.Keyboard.LAltKey = true;
                } else if (key == SDLK_RALT) {
                    Input.Keyboard.RAltKey = true;
                }

                else if (key == SDLK_RIGHT) {
                    Input.Keyboard.RArrowKey = true;
                } else if (key == SDLK_LEFT) {
                    Input.Keyboard.LArrowKey = true;
                } else if (key == SDLK_UP) {
                    Input.Keyboard.UArrowKey = true;
                } else if (key == SDLK_DOWN) {
                    Input.Keyboard.DArrowKey = true;
                }
            } break;
            case SDL_KEYUP: {
                unsigned int key = e.key.keysym.sym;

                if (key >= SDLK_0 && key <= SDLK_9) {
                    Input.Keyboard.numberKeys[key - SDLK_0] = false;
                } else if (key >= SDLK_a && key <= SDLK_z) {
                    Input.Keyboard.letterKeys[key - SDLK_a] = false;
                } else if (key >= SDLK_F1 && key <= SDLK_F12) {
                    Input.Keyboard.FKeys[key - SDLK_F1] = false;
                } else if (key >= SDLK_KP_0 && key <= SDLK_KP_9) {
                    Input.Keyboard.numpadKeys[key - SDLK_KP_0] = false;
                }

                else if (key == SDLK_ESCAPE) {
                    Input.Keyboard.escapeKey = false;
                } else if (key == SDLK_RETURN) {
                    Input.Keyboard.enterKey = false;
                } else if (key == SDLK_SPACE) {
                    Input.Keyboard.spaceKey = false;
                }

                else if (key == SDLK_LSHIFT) {
                    Input.Keyboard.LShiftKey = false;
                } else if (key == SDLK_RSHIFT) {
                    Input.Keyboard.RShiftKey = false;
                } else if (key == SDLK_LCTRL) {
                    Input.Keyboard.LCtrlKey = false;
                } else if (key == SDLK_RCTRL) {
                    Input.Keyboard.RCtrlKey = false;
                } else if (key == SDLK_LALT) {
                    Input.Keyboard.LAltKey = false;
                } else if (key == SDLK_RALT) {
                    Input.Keyboard.RAltKey = false;
                }

                else if (key == SDLK_RIGHT) {
                    Input.Keyboard.RArrowKey = false;
                } else if (key == SDLK_LEFT) {
                    Input.Keyboard.LArrowKey = false;
                } else if (key == SDLK_UP) {
                    Input.Keyboard.UArrowKey = false;
                } else if (key == SDLK_DOWN) {
                    Input.Keyboard.DArrowKey = false;
                }
            } break;
            case SDL_MOUSEMOTION:
                Input.Mouse.x = e.motion.x;
                Input.Mouse.y = e.motion.y;

                Input.Mouse.xRel = e.motion.xrel;
                Input.Mouse.yRel = e.motion.yrel;

                break;

            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    Input.Mouse.lButton = true;
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    Input.Mouse.rButton = true;
                } else if (e.button.button == SDL_BUTTON_MIDDLE) {
                    Input.Mouse.mButton = true;
                }

                break;

            case SDL_MOUSEBUTTONUP:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    Input.Mouse.lButton = false;
                } else if (e.button.button == SDL_BUTTON_RIGHT) {
                    Input.Mouse.rButton = false;
                } else if (e.button.button == SDL_BUTTON_MIDDLE) {
                    Input.Mouse.mButton = false;
                }

                break;
        }
    }
}

void Input_init() { Input.handleEvents = _handleEvents; }
