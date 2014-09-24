#ifndef ROTATION_HPP
#define ROTATION_HPP

#include "Quaternion.hpp"

class Rotation {
public:
	Rotation(const Quaternion& pRotation) : mOld(pRotation), mCurrent(pRotation), mNew(pRotation) {
	}

	void step();

	void increaseRotation(const Quaternion& pNewRotation);
	void setRotation(const Quaternion& pNewRotation);
	void teleport(const Quaternion& pNewRotation);
	const Quaternion& get() const;

	void prepareRotation(float pTime);
	const Quaternion& getCurrentRotation() const;

	const Quaternion estimateRotation(float pTime) const;

	void lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp);
private:
	Quaternion mOld;
	Quaternion mCurrent;
	Quaternion mNew;
};

#endif