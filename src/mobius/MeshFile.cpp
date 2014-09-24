#include "MeshFile.hpp"

#include "File.hpp"
#include "MeshData.hpp"
#include "StringUtils.hpp"
#include "Exceptions.hpp"
#include "AbstractMaterialLibrary.hpp"
#include <cassert>

void MeshFile::increaseUsage(){
	++mUsage;
}
void MeshFile::decreaseUsage(){
	assert(mUsage>0);
	--mUsage;
}
bool MeshFile::isInUse() const {
	return (mUsage > 0);
}

MeshFile::MeshFile( const std::string& pFileName) : mUsage(0) {
	std::istringstream file;
	loadFileIntoString(pFileName, &file);
	std::string line;
	// this will serve as a temporary placement for vertices, normals and vertex normals
	MeshData indexBase;
	std::auto_ptr<MeshData> component;
	AbstractMaterialLibrary materialLibrary;
	bool hasVertices = false;
	unsigned int componentsAdded = 0;
	while( std::getline(file, line) ) {
		std::vector<std::string> splittedLine;
		splitString(" ", line, &splittedLine);
		const std::string id = splittedLine[0];
		if(    id == "v"
			|| id=="vt" 
			|| id=="vn" ) {
			indexBase.handleFileLine(splittedLine);
		}
		else if (id=="f") {
			assert( component.get() );
			if( !hasVertices ) {
				component->gatherDataFrom(indexBase);
				hasVertices = true;
			}
			component->handleFileLine(splittedLine);
		}
		else if(id=="mtllib") {
			materialLibrary.load( trim(splittedLine[1]) );
		}
		else if(id=="usemtl") {
			if( component.get() && hasVertices ) {
				mData.push_back(component.release());
				++componentsAdded;
			}
			const std::string materialName = trim( splittedLine[1] );
			const AbstractMaterial& material = materialLibrary.getMaterial(materialName);
			component.reset(new MeshData() );
			component->setMaterial(material);
			hasVertices = false;
		}
	}
	if( component.get() && hasVertices ) {
		mData.push_back( component.release() );
		++componentsAdded;
	}
	if( 0==componentsAdded ) {
		throw BadDataException("Not enough components in object mesh");
	}
}

MeshFile::~MeshFile() {
	for(MeshDataList::iterator meshDataIterator = mData.begin(); meshDataIterator != mData.end(); ++meshDataIterator) {
		MeshData* data = *meshDataIterator;
		delete data;
	}
	mData.clear();
}

MeshFile::MeshDataList::const_iterator MeshFile::begin() const {
	return mData.begin();
}

MeshFile::MeshDataList::const_iterator MeshFile::end() const {
	return mData.end();
}