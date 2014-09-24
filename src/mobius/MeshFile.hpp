#ifndef MESH_FILE_HPP
#define MESH_FILE_HPP

#include <string>
#include <list>

class MeshData;

class MeshFile {
public:
	MeshFile( const std::string& pFileName);
	~MeshFile();
	typedef std::list<MeshData*> MeshDataList;

	MeshDataList::const_iterator begin() const;
	MeshDataList::const_iterator end() const;

	void increaseUsage();
	void decreaseUsage();
	bool isInUse() const;
private:
	MeshDataList mData;
	unsigned int mUsage;
};

#endif