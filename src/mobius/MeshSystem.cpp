#include "MeshSystem.hpp"

MeshSystem::MeshSystem() : System("mesh") {
}
MeshSystem::~MeshSystem() {
	manager.removeUnusedMeshes();
}