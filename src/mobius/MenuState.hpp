#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <memory>
#include "State.hpp"

class MenuState : public State {
public:
	MenuState();
	~MenuState();

	void onStep(float pTime, unsigned int pDepth);
	void onFrame(float pTime, unsigned int pDepth);
	void onRender(float pTime, unsigned int pDepth);

	void onKey(Key pKey, bool pIsDown);
	void onAxis(Axis pAxis, real pValue);

	void onTop();
	void onNoMoreTop();
private:
	struct MenuStatePimpl;
	std::auto_ptr<MenuStatePimpl> mPimpl;
};

#endif