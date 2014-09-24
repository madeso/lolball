#include "StateManager.hpp"
#include "State.hpp"

#include <cassert>


StateManager::StateManager() {
}
StateManager::~StateManager() {
	removeAllStates();
}

State* StateManager::addState(State* pState) {
	assert(pState);
	assert(! hasState(pState) );
	mStates.push_back(pState);
	return pState;
}

/** @todo fix potential bug, removing of memory that doesn't belong to me - bug if memory is allocated in a different module */
void StateManager::removeState(State* pState) {
	assert(pState);
	mStates.remove(pState);
	delete pState;
}
/** @todo fix potential bug, removing of memory that doesn't belong to me - bug if memory is allocated in a different module */
void StateManager::removeAllStates() {
	for(StateList::reverse_iterator stateIterator = mStates.rbegin(); stateIterator!= mStates.rend(); ++stateIterator) {
		State* state = *stateIterator;
		assert(state);
		delete state;
	}
	mStates.clear();
}
bool StateManager::hasState(const State* pState) const {
	for(StateList::const_iterator stateIterator = mStates.begin(); stateIterator!= mStates.end(); ++stateIterator) {
		State* state = *stateIterator;
		assert(state);
		if( pState == state ) {
			return true;
		}
	}
	return false;
}