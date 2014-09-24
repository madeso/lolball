#include "Component3.hpp"
#include "Object3.hpp"
#include <cassert>
#include "math.hpp"

Component3::Component3(Object3* pOwner) : mOwner(pOwner) {
}
const Object3& Component3::getOwner() const {
	assert(mOwner);
	return *mOwner;
}

bool Component3::isTransparent() const {
	const bool temp = !math::equal(getTransperency(), 1.0f);
	return temp;
}

float Component3::getTransperency() const {
	return mMaterial.getAlpha()*getOwner().getTransperency();
}

void Component3::beginMaterial(RenderMode pRenderMode) const {
	bool doLight = true;
	if( pRenderMode == RM_NO_LIGHT ) doLight = false;
	mMaterial.beginMaterial(doLight, true, getTransperency() );
}
void Component3::endMaterial(RenderMode pRenderMode) const {
	bool doLight = true;
	if( pRenderMode == RM_NO_LIGHT ) doLight = false;
	mMaterial.endMaterial(doLight, true );
}

void Component3::setMaterial(const AbstractMaterial& pMaterial) {
	mMaterial.setMaterial(pMaterial);
}

void Component3::setMaterial(const Material& pMaterial) {
	mMaterial.setMaterial(pMaterial);
}