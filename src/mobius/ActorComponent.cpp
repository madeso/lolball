#include "ActorComponent.hpp"
#include "Actor.hpp"
#include "OpenGL.hpp"
#include <cassert>

ActorComponent::ActorComponent(Actor* pActor) : Component3(pActor), mActor(pActor) {
	assert(mActor);
}
void ActorComponent::render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
	assert(mActor);
	glTranslate(mActor->getPositionObject());
	glRotate(mActor->getRotationObject());
	onRender(pWorld, pTime, pRenderMode);
}