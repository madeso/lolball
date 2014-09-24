#ifndef __FONT_HPP
#define __FONT_HPP

#include <sstream>
#include <string>
#include <memory>

#include "Character.hpp"
#include "vec3.hpp"

struct FontColor {
	FontColor(const vec3& rgb, const real a) : color(rgb), alpha(a) {}
	vec3 color;
	real alpha;
};

class Image;

class Font {
public:
	Font(const std::string& p_font, const float p_fontSize);
	~Font();
	void beginRendering(float x, float y);
	void endRendering();

	template<class _Type>
	Font& operator<<(const _Type& p_type) {
		m_stream.str(""); // clear internal string
		m_stream.clear(); // clear error state
		m_stream << p_type; // stream the data
		if( m_stream.str().length() > 0 ) { // if there is a resulting string
			print(m_stream.str()); // print it
		}
		return (*this);
	}

	Font& operator<<(const FontColor& pColor) {
		setColor(pColor);
		return (*this);
	}

	void setStepSize(const float p_stepSize);

	float getWidth(const std::string& pText);
	float getHeight(const std::string& pText);
protected:
	void print(const std::string& p_string);
	void printCharacter(Character c);
	void setColor(const FontColor& pColor);
private:
	// stream to print to
	std::stringstream m_stream;
	CharacterData m_characterDatas[NUMBER_OF_CHARACTERS];
	int texture;
	float x, y;
	float m_additionalSpace;
	float m_spaceWidth;
	float m_stepSize;
	const float m_fontSize;
	std::auto_ptr<Image> img;
	FontColor mColor;
	float mHeight;
};

#define FPRINT(font, x, y, message)	 {font.beginRendering(x,y); font << message; font.endRendering();}

#endif
