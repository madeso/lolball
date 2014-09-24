#include "StateStack.hpp"
#include "State.hpp"
#include <cassert>
#include <iostream>

StateStack* StateStack::sInstance = 0;

StateStack* StateStack::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}
StateStack& StateStack::getInstance() {
	return *( getInstancePtr() );
}

StateStack::StateStack() : mTopState(0) {
	assert(!sInstance);
	sInstance = this;
}
StateStack::~StateStack() {
	assert(sInstance);
	sInstance = 0;
}

void StateStack::addState(State* pState) {
	assert( pState );
	assert( !hasState(pState->getName()) );
	std::cout << "Stack added: " << pState->getName() << std::endl;
	mStates.push_back(pState);
}
void StateStack::removeState(const std::string pName) {
	State* state = getState(pName);
	assert(state);
	mStates.remove(state);
}

void StateStack::enableState(const std::string& pName) {
	State* state = getState(pName);
	assert(state);
	state->activate();
	updateStates();
}
void StateStack::disableState(const std::string& pName) {
	State* state = getState(pName);
	assert(state);
	state->deactivate();
	updateStates();
}
bool StateStack::isEnabled(const std::string& pName) {
	State* state = getState(pName);
	assert(state);
	return state->isActivated();
}

void StateStack::step(float pTime) {
	if(!mTopState) return;
	mTopState->onStep(pTime, 0);
}
void StateStack::frame(float pTime) {
	if(!mTopState) return;
	mTopState->onFrame(pTime, 0);
}
void StateStack::render(float pTime) {
	if(!mTopState) return;
	mTopState->onRender(pTime, 0);
}
void StateStack::handleKey(Key pKey, bool pIsDown) {
	if(!mTopState) return;
	mTopState->onKey(pKey, pIsDown);
}

void StateStack::handleAxis(Axis pAxis, real pValue) {
	if(!mTopState) return;
	mTopState->onAxis(pAxis, pValue);
}

bool StateStack::hasEnabledStates() const {
	return mTopState != 0;
}

void StateStack::updateStates() {
	State* next = 0;
	State* oldTopState = mTopState;
	mTopState = 0;
	for( StateList::reverse_iterator stateIterator = mStates.rbegin(); stateIterator!=mStates.rend(); ++stateIterator) {
		State* state = *stateIterator;
		if( state->isActivated() ) {
			if( next ) {
				next->setPreviousState(state);
			}
			else {
				mTopState = state;
			}
			next = state;
		}
	}

	if( mTopState != oldTopState ) {
		if( mTopState ) {
			mTopState->onTop();
		}
		if( oldTopState ) {
			oldTopState->onNoMoreTop();
		}
	}
}
State* StateStack::getState(const std::string& pName) {
	for( StateList::iterator stateIterator = mStates.begin(); stateIterator!=mStates.end(); ++stateIterator) {
		State* state = *stateIterator;
		if( pName == state->getName() ) {
			return state;
		}
	}
	return 0;
}
bool StateStack::hasState(const std::string& pName) const {
	for( StateList::const_iterator stateIterator = mStates.begin(); stateIterator!=mStates.end(); ++stateIterator) {
		const State* state = *stateIterator;
		if( pName == state->getName() ) {
			return true;
		}
	}
	return false;
}