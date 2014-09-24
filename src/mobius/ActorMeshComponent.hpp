#ifndef MESH_COMPONENT_HPP
#define MESH_COMPONENT_HPP

#include "ActorComponent.hpp"

class MeshData;

class ActorMeshComponent : public ActorComponent {
public:
	ActorMeshComponent(Actor* pActor, const MeshData* pData);
	void onRender(const World3& pWorld, real pTime, RenderMode& pRenderMode) const;
private:
	const MeshData* mData;
};

#endif