#include "ActorMeshComponent.hpp"
#include <cassert>
#include "MeshData.hpp"

ActorMeshComponent::ActorMeshComponent(Actor* pActor, const MeshData* pData) : ActorComponent(pActor), mData(pData) {
	assert(pData);
	setMaterial( pData->getMaterial() );
}
void ActorMeshComponent::onRender(const World3& pWorld, real pTime, RenderMode& pRenderMode) const {
	mData->render();
}