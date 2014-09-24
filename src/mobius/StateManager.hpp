#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include <list>
class State;

class StateManager {
public:
	StateManager();
	~StateManager();

	State* addState(State* pState);
	void removeState(State* pState);
	void removeAllStates();
private:
	bool hasState(const State* pState) const ;
	typedef std::list<State*> StateList;
	StateList mStates;
};

#endif