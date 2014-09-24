#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Object3.hpp"
#include "Position3.hpp"
#include "Rotation.hpp"

class Actor : public Object3 {
public:
	Actor(const std::string& pName, const vec3& pPosition, const Quaternion& pOrientation, bool pRender);
	virtual void teleport(const vec3& pNewPosition, const Quaternion& pNewOrientation);
	virtual void move(const vec3& pNewPosition);
	virtual void moveTo(const vec3& pNewPosition);
	virtual void increaseRotation(const Quaternion& pRotation);
	virtual void rotateTo(const Quaternion& pRotation);
	void lookAt(const vec3& pTo, const vec3& pUp);

	const vec3& getPosition() const;
	const Quaternion& getRotation() const;

	virtual void update(float pTime);
	virtual void doUpdate(float pTime) = 0;

	void prepareForRendering(float pTime);

	void setupLight(LightSetup* pLightSetup) const;
	virtual void doLight(LightSetup* pLightSetup) const {}

	const Position3& getPositionObject() const;
	const Rotation& getRotationObject() const;

	void loadComponentsFromFile(const std::string& pFileName);

	void debugDraw() const ;
private:
	Rotation mRotation;
	Position3 mPosition;
};

#endif