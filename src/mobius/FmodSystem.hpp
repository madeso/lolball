#ifndef FMOD_SYSTEM_HPP
#define FMOD_SYSTEM_HPP

#include "System.hpp"
#include "fmod.hpp"
#include <string>

class FmodSystem : public System {
public:
	FmodSystem();
	~FmodSystem();

	void tick();

	FMOD::System* getSystem();
	static FmodSystem* getInstancePtr();
	static FmodSystem& getInstance();
#ifdef HEART_SFX
	void playHeart(bool pPlay);
#endif
private:
	void initError(FMOD_RESULT pCause) const;
	FMOD::System* mSystem;

#ifdef HEART_SFX
	FMOD::Sound* mHeart;
	FMOD::Channel* mHeartChannel;
#endif

	static FmodSystem* sInstance;
};

#endif