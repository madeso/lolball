#ifndef KEY_HPP
#define KEY_HPP

enum Key {
	// named keys
	K_BACKSPACE,
	K_TAB,
	K_ESCAPE,
	K_SPACE,
	K_RETURN,
	K_PRINTSCRN,
	K_PAUSE,
	K_DELETE,
	K_EXCLAIM,
	K_QUOTEDBL,
	K_HASH,
	K_DOLLAR,
	K_AMPERSAND,
	K_QUOTE,
	K_LEFTPAREN,
	K_RIGHTPAREN,
	K_ASTERISK,
	K_PLUS,
	K_COMMA,
	K_MINUS,
	K_PERIOD,
	K_SLASH,
	K_COLON,
	K_SEMICOLON,
	K_LESS,
	K_EQUALS,
	K_GREATER,
	K_QUESTION,
	K_AT,
	K_LBRACKET,
	K_RBRACKET,
	K_CARET,
	K_UNDERSCORE,
	K_BACKQUOTE,
	K_CLEAR,
	K_BACKSLASH,

	// Numbers
	K_1, K_2, K_3,
	K_4, K_5, K_6,
	K_7, K_8, K_9,
	K_0, 


	// letters
	K_A, K_B, K_C,
	K_D, K_E, K_F,
	K_G, K_H, K_I,
	K_J, K_K, K_L,
	K_M, K_N, K_O,
	K_P, K_Q, K_R,
	K_S, K_T, K_U,
	K_V, K_W, K_X,
	K_Y, K_Z,

	// keypad
	K_KP_7, K_KP_8, K_KP_9,
	K_KP_4, K_KP_5, K_KP_6,
	K_KP_1, K_KP_2, K_KP_3,
	K_KP_0,
	K_KP_PERIOD,
	K_KP_DIVIDE,
	K_KP_MULTIPLY,
	K_KP_MINUS,
	K_KP_PLUS,
	K_KP_ENTER,
	K_KP_EQUALS, //?

	// arows, and home/end pad
	K_UP,
	K_DOWN,
	K_LEFT,
	K_RIGHT,
	K_INSERT,
	K_HOME,
	K_END,
	K_PAGEUP,
	K_PAGEDOWN,

	// modifiers
	K_NUMLOCK,
	K_CAPSLOCK,
	K_SCROLLOCK,

	// right and left keys
	K_R_ALT,
	K_L_ALT,
	K_R_CTRL,
	K_L_CTRL,
	K_R_SHIFT,
	K_L_SHIFT,

	// function keys
	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_F13,
	K_F14,
	K_F15,

	// mouse events
	KM_LEFT,
	KM_MIDDLE,
	KM_RIGHT,
	KM_WUP,
	KM_WDOWN,

	// joystick buttons
	KJ_JUMP,
	KJ_RESTART,
	KJ_SLOWMO,
	KJ_PAUSE,
	KJ_MENU,

	KS_COUNT,
	KS_UNDEFINED
};

const char* keyToString(const Key p_key);
const bool isValid(const Key p_key);

Key sdlKeyCodeToKey(int p_keyCode);

#endif
