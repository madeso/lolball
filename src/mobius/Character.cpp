#include "Character.hpp"

	bool
	canPrint(char c) {
		switch(convertToSpecialCharacter(c) ) {
		case SC_SPACE:
		case SC_TAB:
			return true;
		case SC_NONE:
			{
				Character ch = convertToCharacter(c);
				if( isValidCharacter(ch) ) {
					switch(ch) {
					case CH_BAD:
						return false;
					default:
						return true;
					}
				}
			}
		}
		
		return false;
	}
	
	bool isValidCharacter(Character c) {
		switch(c) {
		case CH_CAP_A: case CH_CAP_B: case CH_CAP_C: 
		case CH_CAP_D: case CH_CAP_E: case CH_CAP_F: 
		case CH_CAP_G: case CH_CAP_H: case CH_CAP_I: 
		case CH_CAP_J: case CH_CAP_K: case CH_CAP_L: 
		case CH_CAP_M: case CH_CAP_N: case CH_CAP_O: 
		case CH_CAP_P: case CH_CAP_Q: case CH_CAP_R: 
		case CH_CAP_S: case CH_CAP_T: case CH_CAP_U: 
		case CH_CAP_V: case CH_CAP_W: case CH_CAP_X: 
		case CH_CAP_Y: case CH_CAP_Z: 
		case CH_A: case CH_B: case CH_C: case CH_D: 
		case CH_E: case CH_F: case CH_G: case CH_H: 
		case CH_I: case CH_J: case CH_K: case CH_L: 
		case CH_M: case CH_N: case CH_O: case CH_P: 
		case CH_Q: case CH_R: case CH_S: case CH_T: 
		case CH_U: case CH_V: case CH_W: case CH_X: 
		case CH_Y: case CH_Z: 
		case CH_SIGN_BANG: case CH_SIGN_QUOTE: 
		case CH_SIGN_LL_REAL: case CH_SIGN_UNDERSCORE: 
		case CH_SIGN_PERCENT: case CH_SIGN_AMPERSAND: 
		case CH_SIGN_SLASH: case CH_SIGN_L_SOFT_BRACKET: 
		case CH_SIGN_R_SOFT_BRACKET: case CH_SIGN_EQUAL: 
		case CH_SIGN_QUESTION: case CH_SIGN_AT: 
		case CH_SIGN_PUND: case CH_SIGN_DOLLAR: 
		case CH_SIGN_EURO: case CH_SIGN_L_CURLY_BRACKET: 
		case CH_SIGN_L_HARD_BRACKET: case CH_SIGN_R_HARD_BRACKET: 
		case CH_SIGN_R_CURLY_BRACKET: case CH_SIGN_PLUS: 
		case CH_SIGN_PRIME: case CH_SIGN_APOSTROPHE: 
		case CH_SIGN_LESS: case CH_SIGN_MORE: 
		case CH_SIGN_MINUS: case CH_SIGN_HALF: 
		case CH_SIGN_PARAGRAPH: case CH_SIGN_PIPE: 
		case CH_SIGN_POWER: case CH_SIGN_TILDE: 
		case CH_SIGN_COMMA: case CH_SIGN_PERIOD: 
		case CH_SIGN_SEMICOLON: case CH_SIGN_COLON: 
		case CH_SIGN_BACK_SLASH: case CH_SIGN_STAR: 
		case CH_0: case CH_1: case CH_2: case CH_3:
		case CH_4: case CH_5: case CH_6: case CH_7:
		case CH_8: case CH_9: case CH_CURSOR:
		case CH_BAD:
			return true;
		default:
			return false;
		}
	}
	Character convertToCharacter(char c) {
		switch(c) {
		case 'A': return CH_CAP_A; 
		case 'B': return CH_CAP_B; 
		case 'C': return CH_CAP_C; 
		case 'D': return CH_CAP_D; 
		case 'E': return CH_CAP_E; 
		case 'F': return CH_CAP_F; 
		case 'G': return CH_CAP_G; 
		case 'H': return CH_CAP_H; 
		case 'I': return CH_CAP_I; 
		case 'J': return CH_CAP_J; 
		case 'K': return CH_CAP_K; 
		case 'L': return CH_CAP_L; 
		case 'M': return CH_CAP_M; 
		case 'N': return CH_CAP_N; 
		case 'O': return CH_CAP_O; 
		case 'P': return CH_CAP_P; 
		case 'Q': return CH_CAP_Q; 
		case 'R': return CH_CAP_R; 
		case 'S': return CH_CAP_S; 
		case 'T': return CH_CAP_T; 
		case 'U': return CH_CAP_U; 
		case 'V': return CH_CAP_V; 
		case 'W': return CH_CAP_W; 
		case 'X': return CH_CAP_X; 
		case 'Y': return CH_CAP_Y; 
		case 'Z': return CH_CAP_Z; 
			
			// minuscule or lower-case '': return
		case 'a': return CH_A; 
		case 'b': return CH_B; 
		case 'c': return CH_C; 
		case 'd': return CH_D; 
		case 'e': return CH_E; 
		case 'f': return CH_F; 
		case 'g': return CH_G; 
		case 'h': return CH_H; 
		case 'i': return CH_I; 
		case 'j': return CH_J; 
		case 'k': return CH_K; 
		case 'l': return CH_L; 
		case 'm': return CH_M; 
		case 'n': return CH_N; 
		case 'o': return CH_O; 
		case 'p': return CH_P; 
		case 'q': return CH_Q; 
		case 'r': return CH_R; 
		case 's': return CH_S; 
		case 't': return CH_T; 
		case 'u': return CH_U; 
		case 'v': return CH_V; 
		case 'w': return CH_W; 
		case 'x': return CH_X; 
		case 'y': return CH_Y; 
		case 'z': return CH_Z; 
		case '!': return CH_SIGN_BANG; 
		case '\"': return CH_SIGN_QUOTE; 
		case '#': return CH_SIGN_LL_REAL; 
		case '_': return CH_SIGN_UNDERSCORE; 
		case '%': return CH_SIGN_PERCENT; 
		case '&': return CH_SIGN_AMPERSAND; 
		case '/': return CH_SIGN_SLASH; 
		case '(': return CH_SIGN_L_SOFT_BRACKET; 
		case ')': return CH_SIGN_R_SOFT_BRACKET; 
		case '=': return CH_SIGN_EQUAL; 
		case '?': return CH_SIGN_QUESTION; 
		case '@': return CH_SIGN_AT; 
		case '£': return CH_SIGN_PUND; 
		case '$': return CH_SIGN_DOLLAR; 
		case '€': return CH_SIGN_EURO; 
		case '{': return CH_SIGN_L_CURLY_BRACKET; 
		case '[': return CH_SIGN_L_HARD_BRACKET; 
		case ']': return CH_SIGN_R_HARD_BRACKET; 
		case '}': return CH_SIGN_R_CURLY_BRACKET; 
		case '+': return CH_SIGN_PLUS; 
		case '`': return CH_SIGN_PRIME; 
		case '´': return CH_SIGN_APOSTROPHE; 
		case '<': return CH_SIGN_LESS; 
		case '>': return CH_SIGN_MORE; 
		case '-': return CH_SIGN_MINUS; 
		case '½': return CH_SIGN_HALF; 
		case '§': return CH_SIGN_PARAGRAPH; 
		case '|': return CH_SIGN_PIPE; 
		case '^': return CH_SIGN_POWER; 
		case '~': return CH_SIGN_TILDE; 
		case ',': return CH_SIGN_COMMA; 
		case '.': return CH_SIGN_PERIOD; 
		case ';': return CH_SIGN_SEMICOLON; 
		case ':': return CH_SIGN_COLON; 
		case '\\': return CH_SIGN_BACK_SLASH; 
		case '*': return CH_SIGN_STAR;

		case '0': return CH_0;
		case '1': return CH_1;
		case '2': return CH_2;
		case '3': return CH_3;
		case '4': return CH_4;
		case '5': return CH_5;
		case '6': return CH_6;
		case '7': return CH_7;
		case '8': return CH_8;
		case '9': return CH_9;
		
		case CHAR_CURSOR: return CH_CURSOR;
		
		// bad character
		default: return CH_BAD;
		}
	}
	std::string characterNameAsString(Character c) {
#define CASEVALUE_AS_STRING(a) case a: return #a
		switch(c) {
		CASEVALUE_AS_STRING ( CH_CAP_A );
		CASEVALUE_AS_STRING ( CH_CAP_B );
		CASEVALUE_AS_STRING ( CH_CAP_C ); 
		CASEVALUE_AS_STRING ( CH_CAP_D ); 
		CASEVALUE_AS_STRING ( CH_CAP_E ); 
		CASEVALUE_AS_STRING ( CH_CAP_F ); 
		CASEVALUE_AS_STRING ( CH_CAP_G ); 
		CASEVALUE_AS_STRING ( CH_CAP_H ); 
		CASEVALUE_AS_STRING ( CH_CAP_I ); 
		CASEVALUE_AS_STRING ( CH_CAP_J ); 
		CASEVALUE_AS_STRING ( CH_CAP_K ); 
		CASEVALUE_AS_STRING ( CH_CAP_L ); 
		CASEVALUE_AS_STRING ( CH_CAP_M ); 
		CASEVALUE_AS_STRING ( CH_CAP_N ); 
		CASEVALUE_AS_STRING ( CH_CAP_O ); 
		CASEVALUE_AS_STRING ( CH_CAP_P ); 
		CASEVALUE_AS_STRING ( CH_CAP_Q ); 
		CASEVALUE_AS_STRING ( CH_CAP_R ); 
		CASEVALUE_AS_STRING ( CH_CAP_S );
		CASEVALUE_AS_STRING ( CH_CAP_T );
		CASEVALUE_AS_STRING ( CH_CAP_U );
		CASEVALUE_AS_STRING ( CH_CAP_V );
		CASEVALUE_AS_STRING ( CH_CAP_W );
		CASEVALUE_AS_STRING ( CH_CAP_X );
		CASEVALUE_AS_STRING ( CH_CAP_Y );
		CASEVALUE_AS_STRING ( CH_CAP_Z );
		CASEVALUE_AS_STRING ( CH_A );
		CASEVALUE_AS_STRING ( CH_B );
		CASEVALUE_AS_STRING ( CH_C );
		CASEVALUE_AS_STRING ( CH_D );
		CASEVALUE_AS_STRING ( CH_E );
		CASEVALUE_AS_STRING ( CH_F );
		CASEVALUE_AS_STRING ( CH_G );
		CASEVALUE_AS_STRING ( CH_H );
		CASEVALUE_AS_STRING ( CH_I );
		CASEVALUE_AS_STRING ( CH_J );
		CASEVALUE_AS_STRING ( CH_K );
		CASEVALUE_AS_STRING ( CH_L );
		CASEVALUE_AS_STRING ( CH_M );
		CASEVALUE_AS_STRING ( CH_N );
		CASEVALUE_AS_STRING ( CH_O );
		CASEVALUE_AS_STRING ( CH_P );
		CASEVALUE_AS_STRING ( CH_Q );
		CASEVALUE_AS_STRING ( CH_R );
		CASEVALUE_AS_STRING ( CH_S );
		CASEVALUE_AS_STRING ( CH_T );
		CASEVALUE_AS_STRING ( CH_U );
		CASEVALUE_AS_STRING ( CH_V );
		CASEVALUE_AS_STRING ( CH_W );
		CASEVALUE_AS_STRING ( CH_X );
		CASEVALUE_AS_STRING ( CH_Y );
		CASEVALUE_AS_STRING ( CH_Z );
		CASEVALUE_AS_STRING ( CH_SIGN_BANG );
		CASEVALUE_AS_STRING ( CH_SIGN_QUOTE );
		CASEVALUE_AS_STRING ( CH_SIGN_LL_REAL );
		CASEVALUE_AS_STRING ( CH_SIGN_UNDERSCORE );
		CASEVALUE_AS_STRING ( CH_SIGN_PERCENT );
		CASEVALUE_AS_STRING ( CH_SIGN_AMPERSAND );
		CASEVALUE_AS_STRING ( CH_SIGN_SLASH );
		CASEVALUE_AS_STRING ( CH_SIGN_L_SOFT_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_R_SOFT_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_EQUAL );
		CASEVALUE_AS_STRING ( CH_SIGN_QUESTION );
		CASEVALUE_AS_STRING ( CH_SIGN_AT );
		CASEVALUE_AS_STRING ( CH_SIGN_PUND );
		CASEVALUE_AS_STRING ( CH_SIGN_DOLLAR );
		CASEVALUE_AS_STRING ( CH_SIGN_EURO );
		CASEVALUE_AS_STRING ( CH_SIGN_L_CURLY_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_L_HARD_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_R_HARD_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_R_CURLY_BRACKET );
		CASEVALUE_AS_STRING ( CH_SIGN_PLUS );
		CASEVALUE_AS_STRING ( CH_SIGN_PRIME );
		CASEVALUE_AS_STRING ( CH_SIGN_APOSTROPHE );
		CASEVALUE_AS_STRING ( CH_SIGN_LESS );
		CASEVALUE_AS_STRING ( CH_SIGN_MORE );
		CASEVALUE_AS_STRING ( CH_SIGN_MINUS );
		CASEVALUE_AS_STRING ( CH_SIGN_HALF );
		CASEVALUE_AS_STRING ( CH_SIGN_PARAGRAPH );
		CASEVALUE_AS_STRING ( CH_SIGN_PIPE );
		CASEVALUE_AS_STRING ( CH_SIGN_POWER );
		CASEVALUE_AS_STRING ( CH_SIGN_TILDE );
		CASEVALUE_AS_STRING ( CH_SIGN_COMMA );
		CASEVALUE_AS_STRING ( CH_SIGN_PERIOD );
		CASEVALUE_AS_STRING ( CH_SIGN_SEMICOLON );
		CASEVALUE_AS_STRING ( CH_SIGN_COLON );
		CASEVALUE_AS_STRING ( CH_SIGN_BACK_SLASH );
		CASEVALUE_AS_STRING ( CH_SIGN_STAR );

		CASEVALUE_AS_STRING ( CH_0 );
		CASEVALUE_AS_STRING ( CH_1 );
		CASEVALUE_AS_STRING ( CH_2 );
		CASEVALUE_AS_STRING ( CH_3 );
		CASEVALUE_AS_STRING ( CH_4 );
		CASEVALUE_AS_STRING ( CH_5 );
		CASEVALUE_AS_STRING ( CH_6 );
		CASEVALUE_AS_STRING ( CH_7 );
		CASEVALUE_AS_STRING ( CH_8 );
		CASEVALUE_AS_STRING ( CH_9 );

		CASEVALUE_AS_STRING ( CH_CURSOR );

		
		// bad character
		CASEVALUE_AS_STRING ( CH_BAD );
#undef CASEVALUE_AS_STRING
		default: return "unknown value";
		}
	}
	std::string characterAsString(Character c) {
		switch(c) {
		case CH_CAP_A: return "A";
		case CH_CAP_B: return "B";
		case CH_CAP_C: return "C";
		case CH_CAP_D: return "D";
		case CH_CAP_E: return "E";
		case CH_CAP_F: return "F";
		case CH_CAP_G: return "G";
		case CH_CAP_H: return "H";
		case CH_CAP_I: return "I";
		case CH_CAP_J: return "J";
		case CH_CAP_K: return "K";
		case CH_CAP_L: return "L";
		case CH_CAP_M: return "M";
		case CH_CAP_N: return "N";
		case CH_CAP_O: return "O";
		case CH_CAP_P: return "P";
		case CH_CAP_Q: return "Q";
		case CH_CAP_R: return "R";
		case CH_CAP_S: return "S";
		case CH_CAP_T: return "T";
		case CH_CAP_U: return "U";
		case CH_CAP_V: return "V";
		case CH_CAP_W: return "W";
		case CH_CAP_X: return "X";
		case CH_CAP_Y: return "Y";
		case CH_CAP_Z: return "Z";
		case CH_A: return "a";
		case CH_B: return "b";
		case CH_C: return "c";
		case CH_D: return "d";
		case CH_E: return "e";
		case CH_F: return "f";
		case CH_G: return "g";
		case CH_H: return "h";
		case CH_I: return "i";
		case CH_J: return "j";
		case CH_K: return "k";
		case CH_L: return "l";
		case CH_M: return "m";
		case CH_N: return "n";
		case CH_O: return "o";
		case CH_P: return "p";
		case CH_Q: return "q";
		case CH_R: return "r";
		case CH_S: return "s";
		case CH_T: return "t";
		case CH_U: return "u";
		case CH_V: return "v";
		case CH_W: return "w";
		case CH_X: return "x";
		case CH_Y: return "y";
		case CH_Z: return "z";
		case CH_SIGN_BANG: return "!";
		case CH_SIGN_QUOTE: return "\"";
		case CH_SIGN_LL_REAL: return "#";
		case CH_SIGN_UNDERSCORE: return "_";
		case CH_SIGN_PERCENT: return "%";
		case CH_SIGN_AMPERSAND: return "&";
		case CH_SIGN_SLASH: return "/";
		case CH_SIGN_L_SOFT_BRACKET: return "(";
		case CH_SIGN_R_SOFT_BRACKET: return ")";
		case CH_SIGN_EQUAL: return "=";
		case CH_SIGN_QUESTION: return "?";
		case CH_SIGN_AT: return "@";
		case CH_SIGN_PUND: return "£";
		case CH_SIGN_DOLLAR: return "$";
		case CH_SIGN_EURO: return "€";
		case CH_SIGN_L_CURLY_BRACKET: return "{";
		case CH_SIGN_L_HARD_BRACKET: return "[";
		case CH_SIGN_R_HARD_BRACKET: return "]";
		case CH_SIGN_R_CURLY_BRACKET: return "}";
		case CH_SIGN_PLUS: return "+";
		case CH_SIGN_PRIME: return "`";
		case CH_SIGN_APOSTROPHE: return "´";
		case CH_SIGN_LESS: return "<";
		case CH_SIGN_MORE: return ">";
		case CH_SIGN_MINUS: return "-";
		case CH_SIGN_HALF: return "½";
		case CH_SIGN_PARAGRAPH: return "§";
		case CH_SIGN_PIPE: return "|";
		case CH_SIGN_POWER: return "^";
		case CH_SIGN_TILDE: return "~";
		case CH_SIGN_COMMA: return ",";
		case CH_SIGN_PERIOD: return ".";
		case CH_SIGN_SEMICOLON: return ";";
		case CH_SIGN_COLON: return ":";
		case CH_SIGN_BACK_SLASH: return "\\";
		case CH_SIGN_STAR: return "*";

		case CH_0: return "0";
		case CH_1: return "1";
		case CH_2: return "2";
		case CH_3: return "3";
		case CH_4: return "4";
		case CH_5: return "5";
		case CH_6: return "6";
		case CH_7: return "7";
		case CH_8: return "8";
		case CH_9: return "9";
		case CH_CURSOR: return "cursor";
		
		// bad character
		case CH_BAD: return "bad";
		default: return "";
		}
	}

	SpecialCharacter convertToSpecialCharacter(char c) {
		switch(c) {
		case ' ':
			return SC_SPACE;
		case '\t':
			return SC_TAB;
		default:
			return SC_NONE;
		}
	}