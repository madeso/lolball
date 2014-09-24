#include "Game.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "MenuState.hpp"
#include "sdl.h"
#include "Exceptions.hpp"
#include "StateAction.hpp"

int main(int nArgs, char **args) {
	try {
		Game game("entsoft", "lolball", "../config.xml", args[0]);
		game.addState(new MenuState(), SA_ENABLE_STATE);
		game.addState(new GameState(), SA_DISABLE_STATE);
		game.addState(new PauseState(), SA_DISABLE_STATE);
		game.loop( 40,  // 40 ticks between updates
					3); // max 3 updates per render
	}
	catch(...) {
		handleExceptions();
	}
	return 0;
}