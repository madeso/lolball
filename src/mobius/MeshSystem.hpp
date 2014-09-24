#ifndef MESH_SYSTEM_HPP
#define MESH_SYSTEM_HPP

#include "System.hpp"
#include <string>
#include "MeshDataManager.hpp"

class MeshSystem : public System {
public:
	MeshSystem();
	~MeshSystem();
private:
	MeshDataManager manager;
};

#endif