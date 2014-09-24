#ifndef MESH_DATA_MANAGER_HPP
#define MESH_DATA_MANAGER_HPP

#include <string>
#include <map>

class MeshFile;

class MeshDataManager {
public:
	MeshDataManager();
	~MeshDataManager();
	MeshFile* getFile(const std::string& pFileName);
	bool hasFile(const std::string& pFileName) const;
	void removeUnusedMeshes();

	static MeshDataManager* getInstancePtr();
	static MeshDataManager& getInstance();
private:
	typedef std::map<std::string, MeshFile*> StringMeshMap;
	typedef std::pair<std::string, MeshFile*> StringMeshPair;
	StringMeshMap mMeshes;

	static MeshDataManager* sInstance;
};

#endif