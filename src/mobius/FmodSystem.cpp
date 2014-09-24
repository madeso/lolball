#include "FmodSystem.hpp"
#include "Exceptions.hpp"

#include "DataPointer.hpp"

#include <sstream>
#include "fmod_errors.h"
#include <cassert>
#include "math.hpp"

#include "MusicMachine.hpp"

void FmodSystem::initError(FMOD_RESULT pCause) const {
	if (pCause != FMOD_OK) {
		std::ostringstream str;
		str << "Fmod error! (" << pCause << ") " << FMOD_ErrorString(pCause);
		throw InitException(str.str());
	}
}

void FmodSystem::tick() {
	assert(mSystem);
	initError( mSystem->update() );
}

FmodSystem::FmodSystem() : System("fmod"), mSystem(0) {
	assert(!sInstance);
	sInstance = this;
	initError(FMOD::System_Create(&mSystem));
	initError(mSystem->init(100, FMOD_INIT_NORMAL, 0));
	FMOD::ChannelGroup* group = 0;
	initError(mSystem->getMasterChannelGroup(&group));
	DataPointer<real> master("master");
	initError( group->setVolume( master.get() ) );

#ifdef HEART_SFX
	initError(getSystem()->createSound("../sounds/heartbeat.wav", FMOD_DEFAULT, 0, &mHeart));
	initError(mHeart->setLoopCount(-1));
	initError(getSystem()->playSound(FMOD_CHANNEL_FREE, mHeart, true, &mHeartChannel) );
	initError(mHeartChannel->setLoopCount(-1));
#endif
}

#ifdef HEART_SFX
void FmodSystem::playHeart(bool pPlay) {
	int loop = 500;
	initError(mHeartChannel->getLoopCount(&loop));
	initError(mHeartChannel->setPaused(!pPlay));
	if( pPlay ) {
		MusicMachine::getInstance().setVolume(0.5f);
	}
	else {
		MusicMachine::getInstance().setVolume(1.0f);
	}
}
#endif

FmodSystem::~FmodSystem() {
	assert(sInstance);
	sInstance = 0;
	mSystem->release();
}

FMOD::System* FmodSystem::getSystem() {
	assert(mSystem);
	return mSystem;
}
FmodSystem* FmodSystem::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}
FmodSystem& FmodSystem::getInstance() {
	return *getInstancePtr();
}

FmodSystem* FmodSystem::sInstance = 0;