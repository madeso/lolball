#ifndef POSITION3_HPP
#define POSITION3_HPP

#include "vec3.hpp"

class Position3 {
public:
	Position3(const vec3& pPosition) : mOld(pPosition), mCurrent(pPosition), mNew(pPosition) {
	}

	void step();

	void setNewPosition(const vec3& pNewPosition);
	void teleport(const vec3& pNewPosition);
	const vec3& getPosition() const;

	void preparePosition(float pTime);
	const vec3& getCurrentPosition() const;

	const vec3 estimatePosition(float pTime) const;
private:
	vec3 mOld;
	vec3 mCurrent;
	vec3 mNew;
};

#endif