#include "State.hpp"
#include "StateStack.hpp"
#include <cassert>
#include <iostream>

State::State(const std::string& pName) : mName(pName), mPreviousState(0), mActivated(false) {
	std::cout << "[" << mName << "] created." << std::endl;
}
State::~State() {
	std::cout << "[" << mName << "] destroyed." << std::endl;
}

void State::setPreviousState(State* pNewPreviousState) {
	mPreviousState = pNewPreviousState;
}

const std::string& State::getName() const {
	return mName;
}

void State::stepPrevious(float pTime, unsigned int pDepth) {
	if( mPreviousState ) {
		assert(mPreviousState->isActivated());
		mPreviousState->onStep(pTime, pDepth+1);
	}
}

void State::framePrevious(float pTime, unsigned int pDepth) {
	if( mPreviousState ) {
		assert(mPreviousState->isActivated());
		mPreviousState->onFrame(pTime, pDepth+1);
	}
}

void State::renderPrevious(float pTime, unsigned int pDepth) {
	if( mPreviousState ) {
		assert(mPreviousState->isActivated());
		mPreviousState->onRender(pTime, pDepth+1);
	}
}

void State::enableState(const std::string& pName) const {
	StateStack::getInstance().enableState(pName);
}

void State::disableState(const std::string& pName) const {
	StateStack::getInstance().disableState(pName);
}

void State::disableThis() const {
	disableState(getName());
}

bool State::isActivated() const {
	return mActivated;
}
void State::activate() {
	mActivated = true;
}
void State::deactivate() {
	mActivated = false;
}