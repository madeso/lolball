#include "FontSystem.hpp"
#include "OpenGL.hpp"

FontSystem::FontSystem() : System("font") {
	checkOpengl();
	FontManager::getInstance().loadFont("std", "font.txt", 0.3f);
	checkOpengl();
	FontManager::getInstance().setStandardFont("std");
	checkOpengl();
}
