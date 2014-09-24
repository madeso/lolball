#include "Position3.hpp"

void Position3::step() {
	mOld = mNew;
}
void Position3::setNewPosition(const vec3& pNewPosition) {
	mNew = pNewPosition;
}
void Position3::teleport(const vec3& pNewPosition) {
	mOld = pNewPosition;
	mNew = pNewPosition;
}
const vec3& Position3::getPosition() const {
	return mNew;
}

void Position3::preparePosition(float pTime) {
	mCurrent = estimatePosition(pTime);
}
const vec3& Position3::getCurrentPosition() const {
	return mCurrent;
}

const vec3 Position3::estimatePosition(float pTime) const {
	return (mNew-mOld)*pTime + mOld;
}