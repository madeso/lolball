#include "Game.hpp"
#include <iostream>
#include "math.hpp"
#include "Font.hpp"
#include "FontManager.hpp"

// systems
#include "WeakLinkSystem.hpp"
#include "ConfigSystem.hpp"
#include "FileSystem.hpp"
#include "SDLSystem.hpp"
#include "OpenglSystem.hpp"
#include "ImageSystem.hpp"
#include "FontSystem.hpp"
#include "MeshSystem.hpp"
#include "StateSystem.hpp"
#include "FmodSystem.hpp"

#include "MusicMachine.hpp"

#include "DataPointer.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

struct Game::GamePimpl {
	GamePimpl(Game* pGame, const std::string& pCompanyName, const std::string& pGameName, const std::string& pConfigPath, char* args0)
		: weakLinks(),
		  config(pConfigPath),
		  filesys(pCompanyName, pGameName, args0),
		  sdl(pGame, pGameName),
		  ogl(),
		  img(),
		  font(),
		  mesh(),
		  state(),
		  fmod(),
		  music()
	      {
			  music.loadAndPlaySongs();
			  sdl.centerCursor();
	}

	void handleKey(Key pKey, bool pIsDown) {
		state.handleKey(pKey, pIsDown);
	}
	void handleAxis(Axis pAxis, real pValue) {
		state.handleAxis(pAxis, pValue);
	}

	dword getTickCount() {
		return sdl.getTicks();
	}

	void gameTickRun(float pTime) {
		if( !shouldRun() ) return;
		state.step(pTime);
	}
	void independentTickRun(float pTime) {
		if( ! sdl.handleEvents() ) {
			mRunning = false;
		}
		else {
			if( !shouldRun() ) return;
			state.frame(pTime);
			fmod.tick();
			music.step();
		}
	}
	void drawWithInterpolation(float pTime) {
		if( !shouldRun() ) return;
		glClear(GL_COLOR_BUFFER_BIT);
		state.render(pTime);
		setDisplay2d();
#ifndef _DEBUG
		DataPointer<bool> debug("debug");
		if( debug.get() ) {
#pragma message ("Building with custom debug messages")
#endif
			Font& font = FontManager::getInstance().getStandardFont();
			FPRINT(font, 0.01f, 0.95f, "/tick: " << ((mTickOk)?("good"):("bad")) << ", " << mTickTime);
			FPRINT(font, 0.01f, 0.90f, "/frame: " << mFrameTime);
			FPRINT(font, 0.01f, 0.85f, "/render: "<< mRenderTime);
			if( !math::equal( float(mRenderTime) , 0.0f) ) {
				FPRINT(font, 0.01f, 0.80f, "/rfps: "<< (1000.0f/mRenderTime));
			}
#ifndef _DEBUG
		}
#endif
		SDL_GL_SwapBuffers();
	}

	void runGameLoop(const dword TICK_TIME, const int MAX_LOOPS) {
		dword time0;
		dword time1;
		dword frameTime;

		dword latestTickTime = 0;
		dword latestFrameTime = 0;
		dword latestRenderTime = 0;

		mRunning = true;
	
		std::cout << "--- Main loop: started ---" << std::endl;
	
		time0 = getTickCount();
		checkOpengl();
		do {
			time1 = getTickCount();
			frameTime = 0;
			int numLoops = 0;
	
			/*
				The concept behind this is forcing every game logic tick to represent a
				fixed amount of real-time (real-time is the time shown on your wristwatch).
				Of course, ticks better take less CPU time to execute, than the real-time
				it represents.
			*/
			while((time1-time0) > TICK_TIME && numLoops < MAX_LOOPS) {
				dword start = getTickCount();
				gameTickRun(TICK_TIME/1000.0f);
				latestTickTime = getTickCount() - start;
				time0 += TICK_TIME;
				frameTime += TICK_TIME;
				numLoops++;
				// Could this be a good idea? We're not doing it, anyway.
				// time1 = getTickCount();
			}
	
			float percentWithinTick = math::min(1.0f, float(time1-time0)/TICK_TIME);
			{
				dword start = getTickCount();
				independentTickRun(percentWithinTick);
				latestFrameTime = getTickCount() - start;
			}
	
			// if playing solo and game logic takes way too long, discard pending time
			/*if( !mIsNetworkGame && (time1-time0) > TICK_TIME ) {
				time0 = time1 - TICK_TIME;
			}*/
	
			//if(mCanRender) {
			//	float percentWithinTick = min(1.0f, float(time1-time0)/TICK_TIME);
			{
				dword start = getTickCount();
				drawWithInterpolation(percentWithinTick);
				latestRenderTime = getTickCount() - start;
			}

			// i could have send theese to the draw func, but then again, the values wouldn't be in sync
			//   i.e. the render time would always be one frame after the other values
			//   (frame time to the point to be more on the point)
			setLatestTimings(latestTickTime, latestTickTime<TICK_TIME,latestFrameTime, latestRenderTime);
			//}
		} while ( shouldRun() );

		// do not handle loose events, b/c if events
		// are handled when there are no more states in the statestack
		// it will assert
		// sdl.handleEvents();

		std::cout << "--- Main loop: ended -----" << std::endl;
	}

	bool shouldRun() {
		return !state.empty() && mRunning;
	}

	// this fnction keeps data in sync
	void setLatestTimings(dword pTickTime, bool pTickOk, dword pFrameTime, dword pRenderTime) {
		mTickTime = pTickTime;
		mTickOk = pTickOk;
		mFrameTime = pFrameTime;
		mRenderTime = pRenderTime;
	}

	void addState(State* pState, StateAction pStateAction) {
		state.addState(pState, pStateAction);
	}

	WeakLinkSystem weakLinks;
	ConfigSystem config;
	FileSystem filesys;
	SDLSystem sdl;
	OpenglSystem ogl;
	ImageSystem img;
	FontSystem font;
	MeshSystem mesh;
	StateSystem state;
	FmodSystem fmod;
	MusicMachine music;

	dword mFrameTime;
	dword mTickTime;
	bool mTickOk;
	dword mRenderTime;

	bool mRunning;
};

Game::Game(const std::string& pCompanyName, const std::string& pGameName, const std::string& pConfigPath, char* args0) {
	std::cout << "--- Game initializing ----" << std::endl;
	mPimpl.reset( new Game::GamePimpl(this, pCompanyName, pGameName, pConfigPath, args0) );
}

Game::~Game() {
	mPimpl.reset();
	std::cout << "--- Game uninitialized ---" << std::endl;
}

void Game::loop(const dword TICK_TIME, const int MAX_LOOPS) {
	mPimpl->runGameLoop(TICK_TIME, MAX_LOOPS);
}

void Game::addState(State* pState, StateAction pStateAction) {
	mPimpl->addState(pState, pStateAction);
}
void Game::handleKey(Key pKey, bool pIsDown) {
	mPimpl->handleKey(pKey, pIsDown);
}
void Game::handleAxis(Axis pAxis, real pValue) {
	mPimpl->handleAxis(pAxis, pValue);
}