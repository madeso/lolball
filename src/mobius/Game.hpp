#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <string>

class State;

#include "Types.hpp"
#include "math.hpp"

enum StateAction;
enum Key;
enum Axis;

class Game {
public:
	Game(const std::string& pCompanyName,const std::string& pGameName, const std::string& pConfigPath, char* args0);
	~Game();

	void loop(const dword TICK_TIME, const int MAX_LOOPS);

	void addState(State* pState, StateAction pStateAction);

	void handleKey(Key pKey, bool pIsDown);
	void handleAxis(Axis pAxis, real pValue);
private:
	struct GamePimpl;
	std::auto_ptr<GamePimpl> mPimpl;
};

#endif