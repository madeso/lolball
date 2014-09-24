#include "Rotation.hpp"

void Rotation::step() {
	mOld = mNew;
}

void Rotation::increaseRotation(const Quaternion& pNewRotation){
	mNew = pNewRotation * mNew;
}
void Rotation::teleport(const Quaternion& pNewRotation){
	mOld = pNewRotation;
	mNew = pNewRotation;
}
void Rotation::setRotation(const Quaternion& pNewRotation){
	mNew = pNewRotation;
}
const Quaternion& Rotation::get() const{
	return mNew;
}

void Rotation::prepareRotation(float pTime){
	mCurrent = estimateRotation(pTime);
}
const Quaternion& Rotation::getCurrentRotation() const{
	return mCurrent;
}

const Quaternion Rotation::estimateRotation(float pTime) const{
	return mOld.slerp(mNew, pTime);
}

void Rotation::lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp) {
	mNew.lookAt(pFrom, pTo, pUp);
}