#ifndef OBJECT_MESH_COMPONENT_HPP
#define OBJECT_MESH_COMPONENT_HPP

class MeshData;

#include "Component3.hpp"

class ObjectMeshComponent : public Component3 {
public:
	ObjectMeshComponent(Object3* pOwner, MeshData* pMeshData);
	void render(const World3& pWorld, real pTime, RenderMode pRenderMode) const;
private:
	MeshData* mMeshData;
};

#endif