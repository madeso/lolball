#include "MeshDataManager.hpp"

#include "MeshFile.hpp"
#include <cassert>

MeshDataManager* MeshDataManager::sInstance = 0;;

MeshDataManager* MeshDataManager::getInstancePtr() {
	assert(sInstance);
	return sInstance;
}
MeshDataManager& MeshDataManager::getInstance() {
	return *getInstancePtr();
}

MeshDataManager::MeshDataManager() {
	assert(!sInstance);
	sInstance = this;
}
MeshDataManager::~MeshDataManager() {
	assert(sInstance);
	sInstance = 0;
	assert(mMeshes.empty());
}
void MeshDataManager::removeUnusedMeshes() {
	StringMeshMap toRemove;
	for( StringMeshMap::iterator meshIterator=mMeshes.begin(); meshIterator != mMeshes.end(); ++meshIterator) {
		MeshFile* mesh = meshIterator->second;
		assert(mesh);
		if( !mesh->isInUse() ) {
			toRemove.insert( StringMeshPair(meshIterator->first, meshIterator->second) );
		}
	}
	for( StringMeshMap::iterator meshIterator=toRemove.begin(); meshIterator != toRemove.end(); ++meshIterator) {
		MeshFile* mesh = meshIterator->second;
		assert(mesh);
		mMeshes.erase( meshIterator->first );
		delete mesh;
	}
	toRemove.clear();
}
MeshFile* MeshDataManager::getFile(const std::string& pFileName) {
	StringMeshMap::iterator result = mMeshes.find(pFileName);
	if( result != mMeshes.end() ) {
		return result->second;
	}

	MeshFile* mesh = new MeshFile(pFileName);
	mMeshes.insert(StringMeshPair(pFileName, mesh));
	return mesh;
}
bool MeshDataManager::hasFile(const std::string& pFileName) const {
	StringMeshMap::const_iterator result = mMeshes.find(pFileName);
	return ( result != mMeshes.end() );
}