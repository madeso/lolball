#ifndef __CHARACTER_HPP
#define __CHARACTER_HPP

#include <string>

	enum Character {
		// majuscule, capital or upper-case
		CH_CAP_A, CH_CAP_B, CH_CAP_C, CH_CAP_D, CH_CAP_E, CH_CAP_F, 
		CH_CAP_G, CH_CAP_H, CH_CAP_I, CH_CAP_J, CH_CAP_K, CH_CAP_L, 
		CH_CAP_M, CH_CAP_N, CH_CAP_O, CH_CAP_P, CH_CAP_Q, CH_CAP_R, 
		CH_CAP_S, CH_CAP_T, CH_CAP_U, CH_CAP_V, CH_CAP_W, CH_CAP_X, 
		CH_CAP_Y, CH_CAP_Z,
		
		// minuscule or lower-case
		CH_A, CH_B, CH_C, CH_D, CH_E, CH_F, CH_G, CH_H, CH_I, CH_J,
		CH_K, CH_L, CH_M, CH_N, CH_O, CH_P, CH_Q, CH_R, CH_S, CH_T,
		CH_U, CH_V, CH_W, CH_X, CH_Y, CH_Z,

		// signs
		CH_SIGN_BANG, CH_SIGN_QUOTE, CH_SIGN_LL_REAL, CH_SIGN_UNDERSCORE,
		CH_SIGN_PERCENT, CH_SIGN_AMPERSAND, CH_SIGN_SLASH,
		CH_SIGN_L_SOFT_BRACKET, CH_SIGN_R_SOFT_BRACKET, CH_SIGN_EQUAL,
		CH_SIGN_QUESTION, CH_SIGN_AT, CH_SIGN_PUND, CH_SIGN_DOLLAR,
		CH_SIGN_EURO, CH_SIGN_L_CURLY_BRACKET, CH_SIGN_L_HARD_BRACKET,
		CH_SIGN_R_HARD_BRACKET, CH_SIGN_R_CURLY_BRACKET, CH_SIGN_PLUS,
		CH_SIGN_PRIME, CH_SIGN_APOSTROPHE, CH_SIGN_LESS, CH_SIGN_MORE, 
		CH_SIGN_MINUS, CH_SIGN_HALF, CH_SIGN_PARAGRAPH, CH_SIGN_PIPE,
		CH_SIGN_POWER, CH_SIGN_TILDE, CH_SIGN_COMMA, CH_SIGN_PERIOD,
		CH_SIGN_SEMICOLON, CH_SIGN_COLON, CH_SIGN_BACK_SLASH, CH_SIGN_STAR,

		// LL_REALs
		CH_0, CH_1, CH_2, CH_3, CH_4, CH_5, CH_6, CH_7, CH_8, CH_9,
		
		// the blinking cursor when editing...
		CH_CURSOR,

		// bad character
		CH_BAD, 

		NUMBER_OF_CHARACTERS
	};

	enum SpecialCharacter {
		SC_SPACE,
		SC_TAB,

		SC_NONE
	};

	SpecialCharacter convertToSpecialCharacter(char c);

	bool isValidCharacter(Character c);
	Character convertToCharacter(char c);
	std::string characterAsString(Character c);
	std::string characterNameAsString(Character c);

	bool canPrint(char c);

	const char CHAR_CURSOR = 0x1;

	struct CharacterData {
		float width;
		float height;
		float displacement;
		float textureX;
		float textureY;

		float w; float h;
		float up; float left;
		float down; float right;
	};

#endif // __CHARACTER_HPP
