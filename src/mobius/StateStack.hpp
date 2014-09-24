#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

#include <list>
#include <string>

class State;
enum Key;
enum Axis;

#include "math.hpp"

class StateStack {
public:
	StateStack();
	~StateStack();

	void addState(State* pState);
	void removeState(const std::string pName);

	void enableState(const std::string& pName);
	void disableState(const std::string& pName);
	bool isEnabled(const std::string& pName);

	void step(float pTime);
	void frame(float pTime);
	void render(float pTime);

	static StateStack* getInstancePtr();
	static StateStack& getInstance();

	bool hasState(const std::string& pName) const ;

	bool hasEnabledStates() const;

	void handleKey(Key pKey, bool pIsDown);
	void handleAxis(Axis pAxis, real pValue);
private:
	void updateStates();
	State* getState(const std::string& pName);

	typedef std::list<State*> StateList;
	StateList mStates;
	State* mTopState;

	static StateStack* sInstance;
};

#endif