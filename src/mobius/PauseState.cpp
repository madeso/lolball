#include "FontManager.hpp"
#include "Font.hpp"
#include <cassert>
#include "Object2.hpp"
#include "OpenGL.hpp"
#include "PauseState.hpp"
#include "Key.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

class BackgroundPaused : public Object2{
public:
	BackgroundPaused() : Object2(), mFont(FontManager::getInstance().getStandardFont()), alpha(1) {
	}

	bool step(real pTime) {
		alpha -= pTime * 0.25f;
		if( alpha < 0 ) alpha = -1;
		return true;
	}

	void render(real pTime) {
		if( alpha > 0 ) {
			FPRINT(mFont, 0.4f, 0.10f,
				FontColor(vec3(1, 1, 1), alpha) << "Game Paused");
		}
	}
private:
	Font& mFont;
	real alpha;
};

PauseState::PauseState() : State("pause") {
}
PauseState::~PauseState() {
}

void PauseState::onStep(float pTime, unsigned int pDepth) {
	overlay.step(pTime);
	stepPrevious(pTime, pDepth);
}
void PauseState::onFrame(float pTime, unsigned int pDepth) {
}
void PauseState::onRender(float pTime, unsigned int pDepth) {
	renderPrevious(pTime, pDepth);
	overlay.render(pTime);
}
void PauseState::onKey(Key pKey, bool pIsDown) {
	if( (pKey == K_P || pKey == KJ_PAUSE) && pIsDown ) {
		disableThis();
	}
}
void PauseState::onTop() {
	overlay.addObject( new BackgroundPaused() );
}

void PauseState::onNoMoreTop() {
	overlay.removeAllObjects();
}