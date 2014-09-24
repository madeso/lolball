#ifndef PHYS_ACTOR_HPP
#define PHYS_ACTOR_HPP

#include <string>

#include "ode/ode.h"
#include "Actor.hpp"

class PhysActor : public Actor {
public:
	PhysActor(const std::string& pName, const vec3& pPosition, const Quaternion& pRotation, dWorldID pWorld, dSpaceID pSpace, bool pDoRotation, bool pAutoDisable);

	virtual void teleport(const vec3& pNewPosition, const Quaternion& pNewOrientation);
	virtual void move(const vec3& pNewPosition);
	virtual void moveTo(const vec3& pNewPosition);
	virtual void update(float pTime);

	vec3 getSpeed() const;
	void addImpulse(const vec3& pImpulse, real pTime);

	void buildSphere(float pRadius, float pDensity);
	void buildBox(float pSizeX, float pSizeY, float pSizeZ, float pDensity);
	bool collision(const vec3& pDirection);
	real distanceToWorld(const vec3& pDirection);

	void freeze();
	void unfreeze();
private:
	dWorldID mWorld;
	dSpaceID mSpace;
	dBodyID mBody;
	dGeomID mGeom;
	const bool mDoRotation;
};

#endif