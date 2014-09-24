#ifndef SDL_SYSTEM_HPP
#define SDL_SYSTEM_HPP

#include "System.hpp"
#include <string>
#include "Types.hpp"

#include "sdl.h"

class Game;

class SDLSystem : public System {
public:
	SDLSystem(Game* pGame, const std::string& pGameName);
	~SDLSystem();

	dword getTicks();
	// return false to quit, true to contine on
	bool handleEvents();

	// centers cursor, without sending data to the game
	void centerCursor();
protected:
	Game* mGame;
	SDL_Joystick* stick;
	float joy_dx;
	float joy_dy;
};

#endif