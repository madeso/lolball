#ifndef COMPONENT3_HPP
#define COMPONENT3_HPP

#include "Material.hpp"

class Object3;
class World3;

/** @todo update to reflect better approxies */
enum RenderMode {
	RM_NO_LIGHT,
	RM_COLOR_BUFFER_ONLY,
	RM_DEPTH_BUFFER_ONLY,
	RM_COLOR_AND_DEPTH,
};

class Component3 {
public:
	Component3(Object3* pOwner);
	virtual ~Component3() {}
	virtual void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const = 0;
	const Object3& getOwner() const;

	bool isTransparent() const;
	void beginMaterial(RenderMode pRenderMode) const;
	void endMaterial(RenderMode pRenderMode) const;
	void setMaterial(const AbstractMaterial& pMaterial);
	void setMaterial(const Material& pMaterial);
private:
	float getTransperency() const;
	Object3* mOwner;
	Material mMaterial;
	// mMaterial;
};

#endif