#include "MeshData.hpp"
#include <cassert>
#include "OpenGL.hpp"

#include "StringUtils.hpp"
#include "boost/lexical_cast.hpp"

#include "sdl.h"
#include "SDL_opengl.h"

MeshData::MeshData() {
}

void MeshData::setMaterial(const AbstractMaterial& pMaterial) {
	mMaterial.setMaterial(pMaterial);
}

void MeshData::render() const {
	glBegin(GL_TRIANGLES);
	for(FaceList::const_iterator faceIterator = mFaces.begin(); faceIterator!=mFaces.end(); ++faceIterator) {
		const Face& face = *faceIterator;
		for(int i=0; i<3; ++i) {
			const vec3& vertex = getVertex(face.vertex[i]);
			const vec3& normal = getNormal(face.vertex[i]);
			const vec2& tex = getTextCoord(face.vertex[i]);
			glNormal3f(normal.getX(), normal.getY(), normal.getZ());
			glTexCoord2f(tex.getX(), 1-tex.getY());
			glVertex( vertex );
		}
	}
	glEnd();
}

void MeshData::handleFileLine(const std::vector<std::string>& pLine) {
	const std::string& id = pLine[0];
	if( id == "v" ) {
		float x = boost::lexical_cast<float>(trim(pLine[1]));
		float y = boost::lexical_cast<float>(trim(pLine[2]));
		float z = boost::lexical_cast<float>(trim(pLine[3]));
		mVertices.push_back( vec3(	x,
									y,
									z ) );
	}
	else if(id=="vt") {
		mTextureCoords.push_back( vec2( boost::lexical_cast<float>(trim(pLine[1])),
										boost::lexical_cast<float>(trim(pLine[2])) ) );
	}
	else if(id=="vn") {
		mNormals.push_back( vec3(	boost::lexical_cast<float>(trim(pLine[1])),
									boost::lexical_cast<float>(trim(pLine[2])),
									boost::lexical_cast<float>(trim(pLine[3])) ) );
	}
	else if (id=="f") {
		Face face;
		face.vertex[0] = convertToIndexGroup(pLine[1]);
		face.vertex[1] = convertToIndexGroup(pLine[2]);
		face.vertex[2] = convertToIndexGroup(pLine[3]);
		mFaces.push_back(face);
	}
}

/** @todo implement me */
void MeshData::removeUnneededData() {
	// ignore for now
}
void MeshData::gatherDataFrom(const MeshData& pSource) {
	assert(0 != pSource.mVertices.size());
	assert(0 != pSource.mNormals.size());
	assert(0 != pSource.mTextureCoords.size());
	mVertices = pSource.mVertices;
	mNormals = pSource.mNormals;
	mTextureCoords = pSource.mTextureCoords;
}

MeshData::IndexGroup MeshData::convertToIndexGroup(const std::string& pIndexGroup) {
	IndexGroup group;
	std::vector<std::string> indices;
	splitString("/", pIndexGroup, &indices);
	group.vertex = boost::lexical_cast<int>(trim(indices[0]))-1;
	group.textCoord = boost::lexical_cast<int>(trim(indices[1]))-1;
	group.normal = boost::lexical_cast<int>(trim(indices[2]))-1;
	return group;
}

const MeshData::Vertex& MeshData::getVertex(const IndexGroup& pIndex) const {
	return mVertices[pIndex.vertex];
}
const MeshData::Normal& MeshData::getNormal(const IndexGroup& pIndex) const {
	return mNormals[pIndex.normal];
}
const MeshData::TextCoord& MeshData::getTextCoord(const IndexGroup& pIndex) const {
	return mTextureCoords[pIndex.textCoord];
}

const Material& MeshData::getMaterial() const {
	return mMaterial;
}

size_t MeshData::getVertexCount() const {
	return mVertices.size();
}
size_t MeshData::getFaceCount() const {
	return mFaces.size();
}

const MeshData::Face& MeshData::getFace(size_t pIndex) const {
	return mFaces[pIndex];
}
const MeshData::Vertex& MeshData::getVertex(size_t pIndex) const {
	return mVertices[pIndex];
}