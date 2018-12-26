#ifndef SWAN_KEY_EVENT_HPP
#define SWAN_KEY_EVENT_HPP

#include <string>

namespace SWAN
{
    /// @addtogroup Input
    /// @{

    /** @brief An enumeration that represents keys on a keyboard. */
    enum KeyCode : unsigned
    {
	A, ///< A
	B, ///< B
	C, ///< C
	D, ///< D
	E, ///< E
	F, ///< F
	G, ///< G
	H, ///< H
	I, ///< I
	J, ///< J
	K, ///< K
	L, ///< L
	M, ///< M
	N, ///< N
	O, ///< O
	P, ///< P
	Q, ///< Q
	R, ///< R
	S, ///< S
	T, ///< T
	U, ///< U
	V, ///< V
	W, ///< W
	X, ///< X
	Y, ///< Y
	Z, ///< Z

	Grave, ///< ` (key to the left of the 1 key)
	K1, ///< 1
	K2, ///< 2
	K3, ///< 3
	K4, ///< 4
	K5, ///< 5
	K6, ///< 6
	K7, ///< 7
	K8, ///< 8
	K9, ///< 9
	K0, ///< 0
	Minus, ///< \-
	Equals, ///< =
	Backspace, ///< Backspace key

	N1, ///< Numpad 1
	N2, ///< Numpad 2
	N3, ///< Numpad 3
	N4, ///< Numpad 4
	N5, ///< Numpad 5
	N6, ///< Numpad 6
	N7, ///< Numpad 7
	N8, ///< Numpad 8
	N9, ///< Numpad 9
	N0, ///< Numpad 0

	Escape, ///< Escape key, located at the top left of the keyboard
	F1,  ///< F1 Key
	F2,  ///< F2 Key
	F3,  ///< F3 Key
	F4,  ///< F4 Key
	F5,  ///< F5 Key
	F6,  ///< F6 Key
	F7,  ///< F7 Key
	F8,  ///< F8 Key
	F9,  ///< F9 Key
	F10, ///< F10 Key
	F11, ///< F11 Key
	F12, ///< F12 Key 

	Tab, ///< Tab
	CapsLock, ///< CapsLock
	Enter, ///< Enter (also known as Return)
	Spacebar, ///< Spacebar

	Insert, ///< Insert
	Delete, ///< Delete
	Home, ///< Home
	End, ///< End
	PageUp, ///< PageUp
	PageDown, ///< PageDown

	LBracket, ///< \[
	RBracket, ///< \]
	Semicolon, ///< ;
	Apostrophe, ///< '
	Backslash, ///< \\

	Comma, ///< ,
	Period, ///< .
	Slash, ///< /

	LShift, ///< Left shift
	LCtrl, ///< Left control
	LAlt, ///< Left alt
	RShift, ///< Right shift
	RCtrl, ///< Right control
	RAlt, ///< Right alt

	UpArrow, ///< Up arrow key
	DownArrow, ///< Down arrow key
	LeftArrow, ///< Left arrow key
	RightArrow, ///< Right arrow key

	TotalKeys ///< Total amount of keys that are tracked.
    };

    /** @brief Structure, representing a key event.
     *
     * A key event may be a key being pressed, released, repeated or held down.
     * It records the key that the event happened on (Key::Code) and the state of modifier keys on the keyboard 
     * (Key::LShiftHeld, Key::RShiftHeld, Key::LCtrlHeld, Key::RCtrlHeld, Key::LAltHeld, Key::RAltHeld).
     *
     * @note Shift, Control and Alt may appear as keys on their own.
     *
     */
    struct Key {
	KeyCode Code; ///< The actual key code of the given key.

	bool LShiftHeld = false, ///< Whether the left shift key is being held down.
	    LCtrlHeld = false,   ///< Whether the left control key is being held down.
	    LAltHeld = false,    ///< Whether the left alt key is being held down.
	    RShiftHeld = false,  ///< Whether the right shift key is being held down.
	    RCtrlHeld = false,   ///< Whether the right control key is being held down.
	    RAltHeld = false;    ///< Whether the right alt key is being held down.
    };

    /** @brief Does the key code actually represent a character, that can be printed out?
     *
     * @warning Although the function's concept looks an awful lot like std::isprint,
     *          this function will not match what std::isprint would tell you,
     *          since Enter and Tab are both considered to be represented by
     *          ASCII characters ('\\n' and '\\t' respectively).
     *
     * @param kc The key code to check.
     *
     * @return True if key can be represented as an ASCII character, false otherwise.
     */
    constexpr bool RepresentsChar(KeyCode kc)
    {
	return (kc >= A && kc <= Z)
	    || (kc >= K0 && kc <= K9)
	    || (kc >= N0 && kc <= N9)
	    || (kc >= Grave && kc <= Equals)
	    || kc == Tab || kc == Spacebar || kc == Enter
	    || (kc >= LBracket && kc <= Slash)
	    ;
    }

    /** @brief Does the key actually represent a character, that can be printed out?
     *
     * Internally calls RepresentsChar(KeyCode kc) with kc set to k.Code.
     *
     * @param k The key to check.
     *
     * @return Whether the key can be represented as a character.
     */
    constexpr bool RepresentsChar(Key k) { return RepresentsChar(k.Code); }

    /** @brief Retrieves the name of the given key code as a string.
     *
     * @param kc The key code to convert to a string name.
     *
     * @return The name of the key.
     */
    extern std::string GetKeyName(KeyCode kc);

    /** @brief Retrieves the name of the given key code as a string.
     *
     * If any modifier keys are being held down, this function will add them
     * to the the beginning of the name string (e.g. "LShift + W").
     *
     * @param kc The key code to convert to a string name.
     *
     * @return The name of the key.
     */
    extern std::string GetKeyName(Key k);

    /** @brief Convert the key code into an ASCII character if possible.
     *
     * @param kc The key code to convert.
     * @param shiftHeld Whether the shift key is being held down.
     *
     * @return The character the key code represents (if there is one), '\\0' otherwise.
     */
    extern char KeyAsChar(KeyCode kc, bool shiftHeld = false);

    /** @brief Convert the key into an ASCII character if possible.
     *	
     * Internally calls KeyAsChar(KeyCode kc, bool shiftHeld) with kc set to k.Code and
     * shiftHeld set to (k.LShiftHeld || k.RShiftHeld).
     *
     * @param k The key to convert.
     *
     * @return The character the key represents (if there is one), '\\0' otherwise.
     */
    inline char KeyAsChar(Key k) { return KeyAsChar(k.Code, k.LShiftHeld || k.RShiftHeld); }
    /// @}
}

#endif
