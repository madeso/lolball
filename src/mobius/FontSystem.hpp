#ifndef FONT_SYSTEM_HPP
#define FONT_SYSTEM_HPP

#include "System.hpp"
#include <string>

#include "FontManager.hpp"
class FontSystem : public System {
public:
	FontSystem();
private:
	FontManager manager;
};

#endif