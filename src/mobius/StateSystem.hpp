#ifndef STATE_SYSTEM_HPP
#define STATE_SYSTEM_HPP

#include "System.hpp"
#include <string>

#include "StateStack.hpp"
#include "StateManager.hpp"

class State;
enum StateAction;
enum Axis;

class StateSystem : public System {
public:
	StateSystem();

	void addState(State* pState, StateAction pStateAction);

	bool empty();

	void step(float pTime);
	void frame(float pTime);
	void render(float pTime);

	void handleKey(Key pKey, bool pIsDown);
	void handleAxis(Axis pAxis, real pValue);
private:
	StateStack stack;
	StateManager stateMgr;
};

#endif