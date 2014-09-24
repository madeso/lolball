#include "ObjectMeshComponent.hpp"
#include "MeshData.hpp"
#include <cassert>

ObjectMeshComponent::ObjectMeshComponent(Object3* pOwner, MeshData* pMeshData) : Component3(pOwner), mMeshData(pMeshData) {
	assert(pMeshData);
	setMaterial( pMeshData->getMaterial() );
}
void ObjectMeshComponent::render(const World3& pWorld, real pTime, RenderMode pRenderMode) const {
	assert(mMeshData);
	mMeshData->render();
}