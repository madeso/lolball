#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "State.hpp"
#include "World2.hpp"
#include "World3.hpp"
#include "Data.hpp"

class GameState : public State {
public:
	GameState();
	~GameState();

	void onStep(float pTime, unsigned int pDepth);
	void onFrame(float pTime, unsigned int pDepth);
	void onRender(float pTime, unsigned int pDepth);

	void onKey(Key pKey, bool pIsDown);

	void onTop();

	void resetLevel();
	void completeLevel();
	void gameOver();
	void loadNextLevel();

	bool hasWorld();
	int getPoints();
	void onAxis(Axis pAxis, real pValue);
	void unloadLevel();
	void resetScore();

	static GameState* getInstancePtr();
	static GameState& getInstance();

	// load a world when it's safe, usually before a step
	void loadWorld(const std::string& pFile);
	// loads a world at the call
	void doLoadWorld(const std::string& pFile);

	void tellToResetLevel();

	struct GameStatePimpl;
private:
	void doLoadNextLevel();
	World2 hud;
	World3 world;
	real mLatestUpdateTime;
	std::auto_ptr<GameStatePimpl> mPrivateData;

	bool mLeft;
	bool mRight;
	bool mIn;
	bool mOut;
	bool mRotateLeft;
	bool mRotateRight;

	real mDx;
	real mDy;

	Data<real> mRightLeft;
	Data<real> mInOut;

	std::string mLoadWorldString;
	bool mCompletedLevel;
	bool mAllowRestart;

	static GameState* sInstance;
};

#endif