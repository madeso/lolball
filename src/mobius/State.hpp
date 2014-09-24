#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include "math.hpp"
enum Key;
enum Axis;

class State {
public:
	State(const std::string& pName);
	virtual ~State();
	
	virtual void onStep(float pTime, unsigned int pDepth) = 0;
	virtual void onFrame(float pTime, unsigned int pDepth) = 0;
	virtual void onRender(float pTime, unsigned int pDepth) = 0;
	virtual void onKey(Key pKey, bool pIsDown) = 0;
	virtual void onAxis(Axis pAxis, real pValue) {}

	virtual void onTop() {}
	virtual void onNoMoreTop() {}

	//virtual void onKey(Key pKey) {}
	//virtual void onChar(char c) {}
	//virtual void onAxis(AxisType pAxisType, float value ) {}

	void setPreviousState(State* pNewPreviousState);
	const std::string& getName() const;

	bool isActivated() const;
	void activate();
	void deactivate();

	void disableThis() const;
protected:
	void stepPrevious(float pTime, unsigned int pDepth);
	void framePrevious(float pTime, unsigned int pDepth);
	void renderPrevious(float pTime, unsigned int pDepth);

	void enableState(const std::string& pName) const;
	void disableState(const std::string& pName) const;

	const std::string mName;
private:
	State* mPreviousState;
	bool mActivated;
};

#endif