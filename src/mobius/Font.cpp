#include "Font.hpp"
#include <sstream>
#include "File.hpp"
#include "OpenGL.hpp"

#include "sdl.h"
#include "SDL_opengl.h"
#include "Image.hpp"

Font::Font(const std::string& p_font, const float p_fontSize)
: x(0.0f), y(0.0f),
  m_stepSize(0.1f),
  m_fontSize(p_fontSize), mColor(vec3(1,1,1), 1), mHeight(0) {
	const int originalHeight = 512;
	// load the font data
	std::istringstream fontFile;
	loadFileIntoString(p_font, &fontFile);
	std::string imageFile;
	fontFile >> imageFile;

	m_additionalSpace = (5.0f / originalHeight) * p_fontSize;
	m_spaceWidth =		(12.0f/originalHeight) * p_fontSize;
	const float textureSize = 512.0f;
	for(unsigned int character = CH_CAP_A; character < NUMBER_OF_CHARACTERS; ++character) {
		CharacterData& cd = m_characterDatas[character];
		fontFile >> cd.textureX >> cd.textureY >> cd.height >> cd.width >> cd.displacement;
		if( fontFile.eof() ) {
			cd.height = 99; cd.width = 99; cd.textureX = 0; cd.textureY = 0; cd.displacement = 0;
		}
		cd.w = p_fontSize * (cd.width/textureSize);
		cd.h = p_fontSize * (cd.height/textureSize);
		cd.left = cd.textureX  / textureSize;
		cd.up = cd.textureY / textureSize;
		cd.right = (cd.textureX + cd.width) / textureSize;
		cd.down = (cd.textureY + cd.height)/ textureSize;
		cd.displacement = (cd.displacement / textureSize) * p_fontSize;

		mHeight = math::max(cd.h, mHeight);
	}
	// load texture
	checkOpengl();
	img.reset( new Image(imageFile) );
	checkOpengl();
}

void Font::setColor(const FontColor& pColor) {
	mColor = pColor;
}

Font::~Font() {
	// delete texture
}

void Font::beginRendering(float px, float py) {
	glBindTexture(GL_TEXTURE_2D, img->getId());
	glBegin(GL_QUADS);
	x = px; y = py;
}
void Font::print(const std::string& p_string) {
	const std::size_t length = p_string.length();
	glColor4f(mColor.color.getX(), mColor.color.getY(), mColor.color.getZ(), mColor.alpha);
	for(std::size_t stringIndex=0; stringIndex<length; ++stringIndex) {
		const char c = p_string[stringIndex];
		switch(c) {
			case ' ':
				x += m_spaceWidth;
				break;
			default:
				Character ch = convertToCharacter(c);
				printCharacter(ch);
				break;
		}
	}
}

float Font::getWidth(const std::string& pText) {
	float w = 0;
	const std::size_t length = pText.length();
	for(std::size_t stringIndex=0; stringIndex<length; ++stringIndex) {
		const char c = pText[stringIndex];
		switch(c) {
			case ' ':
				w += m_spaceWidth;
				break;
			default:
				Character ch = convertToCharacter(c);
				if( isValidCharacter(ch) ) {
					unsigned int index = ch;
					// change position to reflect m_position
					CharacterData& cd = m_characterDatas[index];
					w += cd.w + m_additionalSpace;
				}
				break;
		}
	}
	return w;
}
float Font::getHeight(const std::string& pText) {
	float h = 0;
	const std::size_t length = pText.length();
	for(std::size_t stringIndex=0; stringIndex<length; ++stringIndex) {
		const char c = pText[stringIndex];
		switch(c) {
			case ' ':
				h = math::max(m_characterDatas[0].h, h);
				break;
			default:
				Character ch = convertToCharacter(c);
				if( isValidCharacter(ch) ) {
					unsigned int index = ch;
					// change position to reflect m_position
					CharacterData& cd = m_characterDatas[index];
					h = math::max(cd.h, h);
				}
				break;
		}
	}
	return h;
}


void Font::endRendering() {
	glEnd();
	mColor.color = vec3(1, 1, 1);
	mColor.alpha = 1;
}
void Font::setStepSize(const float p_stepSize) {
	m_stepSize = p_stepSize;
}

void Font::printCharacter(Character c) {
	// get character index
	if( !isValidCharacter(c) ) {
		return;
	}
	unsigned int index = c;
	// change position to reflect m_position
	CharacterData& cd = m_characterDatas[index];

	const float left = x;
	const float up = y + cd.displacement;
	const float right = left + cd.w;
	const float down = up + cd.h;

	// a --- b
	// |     |
	// |     |
	// d --- c
#define texCoord(a,b) glTexCoord2f( (a), 1-(b) )
	texCoord(cd.left, cd.down);		glVertex2f(left,down);
	texCoord(cd.right, cd.down);	glVertex2f(right,down);
	texCoord(cd.right, cd.up);		glVertex2f(right,up);
	texCoord(cd.left, cd.up);		glVertex2f(left,up);

	x+= cd.w + m_additionalSpace;
}