#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef void(*handleEvents_func)();
#ifndef __cplusplus
typedef unsigned char bool;
#endif // __cplusplus
typedef struct _input _input;

void Input_init();

struct _input{
    struct _keyboard{
        bool letterKeys[26]; ///Ordered A-Z
        bool FKeys[12];		 ///Ordered F1-F12
        bool numberKeys[10]; ///Ordered 0-9
        bool numpadKeys[10]; ///Ordered 0-9
        enum {ARROWKEY_RIGHT, ARROWKEY_LEFT, ARROWKEY_DOWN, ARROWKEY_UP};
        bool arrowKeys[4];

        bool UArrowKey;	bool DArrowKey;
        bool LArrowKey;	bool RArrowKey;

        bool spaceKey;
        bool escapeKey;

        bool LShiftKey;
        bool RShiftKey;

        bool LCtrlKey; bool LAltKey;
        bool RCtrlKey; bool RAltKey;

        bool enterKey;
    } Keyboard;

    struct _mouse{
        int x,y;
        int xRel,yRel;
        bool lButton, rButton, mButton;
    } Mouse;

    struct _window{
        bool exitRequest;
    } Window;

    handleEvents_func handleEvents;
};

extern _input Input;

#endif // INPUT_H
