#include "StateSystem.hpp"

#include "State.hpp"
#include <cassert>
#include "StateAction.hpp"

StateSystem::StateSystem() : System("state") {
}

void StateSystem::addState(State* pState, StateAction pStateAction) {
	assert(pState);
	StateStack::getInstance().addState( stateMgr.addState( pState ) );
	// since states are disabled by default, we only need to enable them
	if( SA_ENABLE_STATE == pStateAction ) {
		StateStack::getInstance().enableState( pState->getName() );
	}
}

bool StateSystem::empty() {
	return !stack.hasEnabledStates();
}

void StateSystem::step(float pTime) {
	stack.step(pTime);
}
void StateSystem::frame(float pTime) {
	stack.frame(pTime);
}
void StateSystem::render(float pTime) {
	stack.render(pTime);
}
void StateSystem::handleKey(Key pKey, bool pIsDown) {
	stack.handleKey(pKey, pIsDown);
}

void StateSystem::handleAxis(Axis pAxis, real pValue) {
	stack.handleAxis(pAxis, pValue);
}