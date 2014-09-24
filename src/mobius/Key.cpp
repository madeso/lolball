#include "Key.hpp"
#include "sdl.h"


const bool isValid(const Key p_key) {
	switch(p_key) {
		case K_BACKSPACE:	case K_TAB:			case K_ESCAPE:		case K_SPACE:
		case K_RETURN:		case K_PRINTSCRN:	case K_PAUSE:		case K_DELETE:
		case K_EXCLAIM:		case K_QUOTEDBL:	case K_HASH:		case K_DOLLAR:
		case K_AMPERSAND:	case K_QUOTE:		case K_LEFTPAREN:	case K_RIGHTPAREN:
		case K_ASTERISK:	case K_PLUS:		case K_COMMA:		case K_MINUS:
		case K_PERIOD:		case K_SLASH:		case K_COLON:		case K_SEMICOLON:
		case K_LESS:		case K_EQUALS:		case K_GREATER:		case K_QUESTION:
		case K_AT:			case K_LBRACKET:	case K_RBRACKET:	case K_CARET:
		case K_UNDERSCORE:	case K_BACKQUOTE:	case K_CLEAR:		case K_BACKSLASH:
		case K_1:			case K_2:			case K_3:			case K_4:
		case K_5:			case K_6:			case K_7:			case K_8:
		case K_9:			case K_0:			case K_A:			case K_B:
		case K_C:			case K_D:			case K_E:			case K_F:
		case K_G:			case K_H:			case K_I:			case K_J:
		case K_K:			case K_L:			case K_M:			case K_N:
		case K_O:			case K_P:			case K_Q:			case K_R:
		case K_S:			case K_T:			case K_U:			case K_V:
		case K_W:			case K_X:			case K_Y:			case K_Z:
		case K_KP_1:		case K_KP_2:		case K_KP_3:		case K_KP_4:
		case K_KP_5:		case K_KP_6:		case K_KP_7:		case K_KP_8:
		case K_KP_9:		case K_KP_0:		case K_KP_PERIOD:	case K_KP_DIVIDE:
		case K_KP_MULTIPLY:	case K_KP_MINUS:	case K_KP_PLUS:		case K_KP_ENTER:
		case K_KP_EQUALS:	case K_UP:			case K_DOWN:		case K_LEFT:
		case K_RIGHT:		case K_INSERT:		case K_HOME:		case K_END:
		case K_PAGEUP:		case K_PAGEDOWN:	case K_NUMLOCK:		case K_CAPSLOCK:
		case K_SCROLLOCK:	case K_R_ALT:		case K_L_ALT:		case K_R_CTRL:
		case K_L_CTRL:		case K_R_SHIFT:		case K_L_SHIFT:		case K_F1:
		case K_F2:			case K_F3:			case K_F4:			case K_F5:
		case K_F6:			case K_F7:			case K_F8:			case K_F9:
		case K_F10:			case K_F11:			case K_F12:			case K_F13:
		case K_F14:			case K_F15:			case KM_LEFT:		case KM_MIDDLE:
		case KM_RIGHT:		case KM_WUP:		case KM_WDOWN:
			return true;
		default:
			return false;
	}
}

const char* keyToString(const Key p_key) {
	switch(p_key) {
		case K_BACKSPACE:	return "Backspace";
		case K_TAB:			return "Tab";
		case K_ESCAPE:		return "Esc";
		case K_SPACE:		return "Space";
		case K_RETURN:		return "Return";
		case K_PRINTSCRN:	return "PrintScrn";
		case K_PAUSE:		return "Pause";
		case K_DELETE:		return "Delete";
		case K_EXCLAIM:		return "Exclaim";
		case K_QUOTEDBL:	return "Dbl Quote";
		case K_HASH:		return "Hash";
		case K_DOLLAR:		return "Dollar";
		case K_AMPERSAND:	return "Ampersand";
		case K_QUOTE:		return "Quote";
		case K_LEFTPAREN:	return "Left Paren";
		case K_RIGHTPAREN:	return "Right Paren";
		case K_ASTERISK:	return "Asterix";
		case K_PLUS:		return "Plus";
		case K_COMMA:		return "Comma";
		case K_MINUS:		return "Minus";
		case K_PERIOD:		return "Period";
		case K_SLASH:		return "Slash";
		case K_COLON:		return "Colon";
		case K_SEMICOLON:	return "SemiColon";
		case K_LESS:		return "Less";
		case K_EQUALS:		return "Equals";
		case K_GREATER:		return "Greater";
		case K_QUESTION:	return "Question";
		case K_AT:			return "At";
		case K_LBRACKET:	return "Left Bracket";
		case K_RBRACKET:	return "Right Bracket";
		case K_CARET:		return "Caret";
		case K_UNDERSCORE:	return "Underscore";
		case K_BACKQUOTE:	return "Backquote";
		case K_CLEAR:		return "Clear";
		case K_BACKSLASH:	return "Backslash";
		case K_1:			return "1";
		case K_2:			return "2";
		case K_3:			return "3";
		case K_4:			return "4";
		case K_5:			return "5";
		case K_6:			return "6";
		case K_7:			return "7";
		case K_8:			return "8";
		case K_9:			return "9";
		case K_0:			return "0"; 
		case K_A:			return "A";
		case K_B:			return "B";
		case K_C:			return "C";
		case K_D:			return "D";
		case K_E:			return "E";
		case K_F:			return "F";
		case K_G:			return "G";
		case K_H:			return "H";
		case K_I:			return "I";
		case K_J:			return "J";
		case K_K:			return "K";
		case K_L:			return "L";
		case K_M:			return "M";
		case K_N:			return "N";
		case K_O:			return "O";
		case K_P:			return "P";
		case K_Q:			return "Q";
		case K_R:			return "R";
		case K_S:			return "S";
		case K_T:			return "T";
		case K_U:			return "U";
		case K_V:			return "V";
		case K_W:			return "W";
		case K_X:			return "X";
		case K_Y:			return "Y";
		case K_Z:			return "Z";
		case K_KP_1:		return "Keypad 1";
		case K_KP_2:		return "Keypad 2";
		case K_KP_3:		return "Keypad 3";
		case K_KP_4:		return "Keypad 4";
		case K_KP_5:		return "Keypad 5";
		case K_KP_6:		return "Keypad 6";
		case K_KP_7:		return "Keypad 7";
		case K_KP_8:		return "Keypad 8";
		case K_KP_9:		return "Keypad 9";
		case K_KP_0:		return "Keypad 0";
		case K_KP_PERIOD:	return "Keypad Period";
		case K_KP_DIVIDE:	return "Keypad Divide";
		case K_KP_MULTIPLY:	return "Keypad Multiply";
		case K_KP_MINUS:	return "Keypad Minus";
		case K_KP_PLUS:		return "Keypad Plus";
		case K_KP_ENTER:	return "Keypad Enter";
		case K_KP_EQUALS:	return "Keypad Equals";
		case K_UP:			return "Up";
		case K_DOWN:		return "Down";
		case K_LEFT:		return "Left";
		case K_RIGHT:		return "Right";
		case K_INSERT:		return "Insert";
		case K_HOME:		return "Home";
		case K_END:			return "End";
		case K_PAGEUP:		return "Page up";
		case K_PAGEDOWN:	return "Page down";
		case K_NUMLOCK:		return "Num lock";
		case K_CAPSLOCK:	return "Caps lock";
		case K_SCROLLOCK:	return "Scroll lock";
		case K_R_ALT:		return "Right Alt";
		case K_L_ALT:		return "Left Alt";
		case K_R_CTRL:		return "Right Ctrl";
		case K_L_CTRL:		return "Left Ctrl";
		case K_R_SHIFT:		return "Right Shift";
		case K_L_SHIFT:		return "Left Shift";
		case K_F1:			return "F1";
		case K_F2:			return "F2";
		case K_F3:			return "F3";
		case K_F4:			return "F4";
		case K_F5:			return "F5";
		case K_F6:			return "F6";
		case K_F7:			return "F7";
		case K_F8:			return "F8";
		case K_F9:			return "F9";
		case K_F10:			return "F10";
		case K_F11:			return "F11";
		case K_F12:			return "F12";
		case K_F13:			return "F13";
		case K_F14:			return "F14";
		case K_F15:			return "F15";
		case KM_LEFT:		return "Left Mouse-button";
		case KM_MIDDLE:		return "Middle Mouse-button";
		case KM_RIGHT:		return "Right Mouse-button";
		case KM_WUP:		return "Mouse-wheel Up";
		case KM_WDOWN:		return "Mouse-wheel Down";
		case KS_UNDEFINED:
		default:
			return "???";
	}
}

Key sdlKeyCodeToKey(int p_keyCode) {
	switch( p_keyCode ) {
	case SDLK_BACKSPACE:		return K_BACKSPACE			;
	case SDLK_TAB:				return K_TAB			;
	case SDLK_CLEAR:			return K_CLEAR			;
	case SDLK_RETURN:			return K_RETURN			;
	case SDLK_PAUSE:			return K_PAUSE			;
	case SDLK_ESCAPE:			return K_ESCAPE		;
	case SDLK_SPACE:			return K_SPACE			;
	case SDLK_EXCLAIM:			return K_EXCLAIM			;
	case SDLK_QUOTEDBL:			return K_QUOTEDBL			;
	case SDLK_HASH:				return K_HASH			;
	case SDLK_DOLLAR:			return K_DOLLAR			;
	case SDLK_AMPERSAND:		return K_AMPERSAND			;
	case SDLK_QUOTE:			return K_QUOTE			;
	case SDLK_LEFTPAREN:		return K_LEFTPAREN			;
	case SDLK_RIGHTPAREN:		return K_RIGHTPAREN			;
	case SDLK_ASTERISK:			return K_ASTERISK			;
	case SDLK_PLUS:				return K_PLUS			;
	case SDLK_COMMA:			return K_COMMA			;
	case SDLK_MINUS:			return K_MINUS			;
	case SDLK_PERIOD:			return K_PERIOD			;
	case SDLK_SLASH:			return K_SLASH			;
	case SDLK_0:				return K_0			;
	case SDLK_1:				return K_1			;
	case SDLK_2:				return K_2			;
	case SDLK_3:				return K_3			;
	case SDLK_4:				return K_4			;
	case SDLK_5:				return K_5			;
	case SDLK_6:				return K_6			;
	case SDLK_7:				return K_7			;
	case SDLK_8:				return K_8			;
	case SDLK_9:				return K_9			;
	case SDLK_COLON:			return K_COLON			;
	case SDLK_SEMICOLON:		return K_SEMICOLON			;
	case SDLK_LESS:				return K_LESS			;
	case SDLK_EQUALS:			return K_EQUALS			;
	case SDLK_GREATER:			return K_GREATER			;
	case SDLK_QUESTION:			return K_QUESTION			;
	case SDLK_AT:				return K_AT			;
	case SDLK_LEFTBRACKET:		return K_LBRACKET			;
	case SDLK_BACKSLASH:		return K_BACKSLASH			;
	case SDLK_RIGHTBRACKET:		return K_RBRACKET			;
	case SDLK_CARET:			return K_CARET			;
	case SDLK_UNDERSCORE:		return K_UNDERSCORE			;
	case SDLK_BACKQUOTE:		return K_BACKQUOTE			;
	case SDLK_a:				return K_A			;
	case SDLK_b:				return K_B			;
	case SDLK_c:				return K_C			;
	case SDLK_d:				return K_D			;
	case SDLK_e:				return K_E			;
	case SDLK_f:				return K_F			;
	case SDLK_g:				return K_G			;
	case SDLK_h:				return K_H			;
	case SDLK_i:				return K_I			;
	case SDLK_j:				return K_J			;
	case SDLK_k:				return K_K			;
	case SDLK_l:				return K_L			;
	case SDLK_m:				return K_M			;
	case SDLK_n:				return K_N			;
	case SDLK_o:				return K_O			;
	case SDLK_p:				return K_P			;
	case SDLK_q:				return K_Q			;
	case SDLK_r:				return K_R			;
	case SDLK_s:				return K_S			;
	case SDLK_t:				return K_T			;
	case SDLK_u:				return K_U			;
	case SDLK_v:				return K_V			;
	case SDLK_w:				return K_W			;
	case SDLK_x:				return K_X			;
	case SDLK_y:				return K_Y			;
	case SDLK_z:				return K_Z			;
	case SDLK_DELETE:			return K_DELETE			;
	case SDLK_KP0:				return K_KP_0			;
	case SDLK_KP1:				return K_KP_1			;
	case SDLK_KP2:				return K_KP_2			;
	case SDLK_KP3:				return K_KP_3			;
	case SDLK_KP4:				return K_KP_4			;
	case SDLK_KP5:				return K_KP_5			;
	case SDLK_KP6:				return K_KP_6			;
	case SDLK_KP7:				return K_KP_7			;
	case SDLK_KP8:				return K_KP_8			;
	case SDLK_KP9:				return K_KP_9			;
	case SDLK_KP_PERIOD:		return K_KP_PERIOD			;
	case SDLK_KP_DIVIDE:		return K_KP_DIVIDE			;
	case SDLK_KP_MULTIPLY:		return K_KP_MULTIPLY			;
	case SDLK_KP_MINUS:			return K_KP_MINUS			;
	case SDLK_KP_PLUS:			return K_KP_PLUS			;
	case SDLK_KP_ENTER:			return K_KP_ENTER			;
	case SDLK_KP_EQUALS:		return K_KP_EQUALS			;
	case SDLK_UP:				return K_UP			;
	case SDLK_DOWN:				return K_DOWN			;
	case SDLK_RIGHT:			return K_RIGHT			;
	case SDLK_LEFT:				return K_LEFT			;
	case SDLK_INSERT:			return K_INSERT			;
	case SDLK_HOME:				return K_HOME			;
	case SDLK_END:				return K_END			;
	case SDLK_PAGEUP:			return K_PAGEUP			;
	case SDLK_PAGEDOWN:			return K_PAGEDOWN			;
	case SDLK_F1:				return K_F1			;
	case SDLK_F2:				return K_F2			;
	case SDLK_F3:				return K_F3			;
	case SDLK_F4:				return K_F4			;
	case SDLK_F5:				return K_F5			;
	case SDLK_F6:				return K_F6			;
	case SDLK_F7:				return K_F7			;
	case SDLK_F8:				return K_F8			;
	case SDLK_F9:				return K_F9			;
	case SDLK_F10:				return K_F10			;
	case SDLK_F11:				return K_F11			;
	case SDLK_F12:				return K_F12			;
	case SDLK_F13:				return K_F13			;
	case SDLK_F14:				return K_F14			;
	case SDLK_F15:				return K_F15			;
	case SDLK_NUMLOCK:			return K_NUMLOCK			;
	case SDLK_CAPSLOCK:			return K_CAPSLOCK			;
	case SDLK_SCROLLOCK:		return K_SCROLLOCK		;
	case SDLK_RSHIFT:			return K_R_SHIFT			;
	case SDLK_LSHIFT:			return K_L_SHIFT			;
	case SDLK_RCTRL:			return K_R_CTRL			;
	case SDLK_LCTRL:			return K_L_CTRL			;
	case SDLK_RALT:				return K_R_ALT			;
	case SDLK_LALT:				return K_L_ALT			;
	case SDLK_SYSREQ:			return K_PRINTSCRN			;
	}

	return KS_UNDEFINED;
}