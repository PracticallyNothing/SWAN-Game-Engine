#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef void (*handleEvents_func)();
#ifndef __cplusplus
typedef unsigned char bool;
#endif // __cplusplus
typedef struct _input _input;

void SWAN_Input_Init();

struct _input {
	struct _keyboard {
		bool letterKeys[26]; // Ordered A-Z
		bool FKeys[12];      // Ordered F1-F12
		bool numberKeys[10]; // Ordered 0-9
		bool numpadKeys[10]; // Ordered 0-9

		bool UArrowKey, DArrowKey, LArrowKey, RArrowKey;

		bool LShiftKey, RShiftKey, LCtrlKey, LAltKey, RCtrlKey, RAltKey;

		bool spaceKey, escapeKey, enterKey;
	} Keyboard;

	struct _mouse {
		int x, y;
		int xRel, yRel;
		bool lButton, rButton, mButton;
	} Mouse;

	struct _window {
		bool exitRequest;
	} Window;

	handleEvents_func handleEvents;
};

extern _input SWAN_Input;

namespace SWAN {
namespace detail {
	extern _input prevState;
}

struct MouseState {
	int x, y;
	bool LMB, MMB, RMB;

	bool mousesOver(int x, int y, int w, int h) {
		return this->x > x && this->y > y && this->x < x + w && this->y < y + h;
	}
};

extern MouseState GetCurrMouseState();
extern MouseState GetPrevMouseState();
}

#endif // INPUT_H
