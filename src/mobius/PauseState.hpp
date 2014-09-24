#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "State.hpp"
#include "World2.hpp"

class PauseState : public State {
public:
	PauseState();
	~PauseState();

	void onStep(float pTime, unsigned int pDepth);
	void onFrame(float pTime, unsigned int pDepth);
	void onRender(float pTime, unsigned int pDepth);

	void onKey(Key pKey, bool pIsDown);

	void onTop();
	void onNoMoreTop();
private:
	World2 overlay;
};

#endif