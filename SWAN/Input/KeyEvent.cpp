#include "KeyEvent.hpp"

namespace SWAN
{
    char KeyAsChar(KeyCode kc, bool shiftHeld)
    {
	switch(kc)
	{
	case A: case B: case C: case D: case E: case F:
	case G: case H: case I: case J: case K: case L:
	case M: case N: case O: case P: case Q: case R:
	case S: case T: case U: case V: case W: case X:
	case Y: case Z:
	    return (shiftHeld ? 'A' : 'a') + (kc - A);

	case Grave:
	case K1: case K2: case K3: case K4: case K5:
	case K6: case K7: case K8: case K9: case K0: 
	case Minus: case Equals: 
	    return (shiftHeld
		    ? "~!@#$%^&*()_+"
		    : "`1234567890-=")[kc - Grave];

	case N1: case N2: case N3: case N4: case N5:
	case N6: case N7: case N8: case N9: case N0:
	    return "1234567890"[(kc - N1)];

	case Tab: return '\t';
	case Enter: return '\n';
	case Spacebar: return ' ';

	case LBracket: return shiftHeld ? '{' : '[';
	case RBracket: return shiftHeld ? '}' : ']';
	case Semicolon: return shiftHeld ? ':' : ';';
	case Apostrophe: return shiftHeld ? '"' : '\'';
	case Backslash: return shiftHeld ? '|' : '\\';
	case Comma: return shiftHeld ? '<' : ',';
	case Period: return shiftHeld ? '>' : '.';
	case Slash: return shiftHeld ? '?' : '/';

	default: return '\0';
	}
    }

    std::string GetKeyName(KeyCode kc)
    {
	switch(kc) {
	case A: return "A";
	case B: return "B";
	case C: return "C";
	case D: return "D";
	case E: return "E";
	case F: return "F";
	case G: return "G";
	case H: return "H";
	case I: return "I";
	case J: return "J";
	case K: return "K";
	case L: return "L";
	case M: return "M";
	case N: return "N";
	case O: return "O";
	case P: return "P";
	case Q: return "Q";
	case R: return "R";
	case S: return "S";
	case T: return "T";
	case U: return "U";
	case V: return "V";
	case W: return "W";
	case X: return "X";
	case Y: return "Y";
	case Z: return "Z";

	case Grave: return "Backtick";
	case K1: return "1";
	case K2: return "2";
	case K3: return "3";
	case K4: return "4";
	case K5: return "5";
	case K6: return "6";
	case K7: return "7";
	case K8: return "8";
	case K9: return "9";
	case K0: return "0";
	case Minus: return "-";
	case Equals: return "=";
	case Backspace: return "Backspace";

	case N1: return "Numpad 1";
	case N2: return "Numpad 2";
	case N3: return "Numpad 3";
	case N4: return "Numpad 4";
	case N5: return "Numpad 5";
	case N6: return "Numpad 6";
	case N7: return "Numpad 7";
	case N8: return "Numpad 8";
	case N9: return "Numpad 9";
	case N0: return "Numpad 0";

	case Escape: return "Escape";
	case F1: return "F1";
	case F2: return "F2";
	case F3: return "F3";
	case F4: return "F4";
	case F5: return "F5";
	case F6: return "F6";
	case F7: return "F7";
	case F8: return "F8";
	case F9: return "F9";
	case F10: return "F10";
	case F11: return "F11";
	case F12: return "F12 ";

	case Tab: return "Tab";
	case CapsLock: return "CapsLock";
	case Enter: return "Enter";
	case Spacebar: return "Spacebar";

	case Insert: return "Insert";
	case Delete: return "Delete";
	case Home: return "Home";
	case End: return "End";
	case PageUp: return "PageUp";
	case PageDown: return "PageDown";

	case LBracket: return "[";
	case RBracket: return "]";
	case Semicolon: return ";";
	case Apostrophe: return "'";
	case Backslash: return "\\";

	case Comma: return ",";
	case Period: return ".";
	case Slash: return "/";

	case LShift: return "LShift";
	case LCtrl: return "LCtrl";
	case LAlt: return "LAlt";
	case RShift: return "RShift";
	case RCtrl: return "RCtrl";
	case RAlt: return "RAlt";

	case UpArrow: return "Up arrow key";
	case DownArrow: return "Down arrow key";
	case LeftArrow: return "Left arrow key";
	case RightArrow: return "Right arrow key";
	default: return "UNKNOWN KEY";
	}
    }

    std::string GetKeyName(Key k)
    {
	std::string name = GetKeyName(k.Code);
	std::string modifiers;

	if(k.Code != LShift && k.LShiftHeld) modifiers += "LShift + ";
	if(k.Code != RShift && k.RShiftHeld) modifiers += "RShift + ";
	if(k.Code != LCtrl && k.LCtrlHeld) modifiers += "LCtrl + ";
	if(k.Code != RCtrl && k.RCtrlHeld) modifiers += "RCtrl + ";
	if(k.Code != LAlt && k.LAltHeld) modifiers += "LAlt + ";
	if(k.Code != RAlt && k.RAltHeld) modifiers += "RAlt + ";

	return modifiers + name;
    }
}
