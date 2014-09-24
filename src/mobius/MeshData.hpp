#ifndef MESH_DATA_HPP
#define MESH_DATA_HPP

#include "vec3.hpp"
#include "vec2.hpp"

#include <vector>
#include <list>

#include "Material.hpp"

struct AbstractMaterial;

class MeshData {
public:
	MeshData();

	void render() const;

	void handleFileLine(const std::vector<std::string>& pLine);
	void removeUnneededData();
	void gatherDataFrom(const MeshData& pSource);

	void setMaterial(const AbstractMaterial& pMaterial);
	const Material& getMaterial() const;

	typedef vec3 Vertex;
	typedef vec3 Normal;
	typedef vec2 TextCoord;
	typedef unsigned int Index;
	struct IndexGroup { Index vertex; Index normal; Index textCoord; };
	struct Face { IndexGroup vertex[3]; };

	size_t getVertexCount() const;
	size_t getFaceCount() const;

	const Face& getFace(size_t pIndex) const;
	const Vertex& getVertex(size_t pIndex) const;
private:
	typedef std::vector<Vertex> VertexList;
	typedef std::vector<Normal> NormalList;
	typedef std::vector<TextCoord> TextList;
	typedef std::vector<Face> FaceList;

	static IndexGroup convertToIndexGroup(const std::string& pIndexGroup);

	const Vertex& getVertex(const IndexGroup& pIndex) const;
	const Normal& getNormal(const IndexGroup& pIndex) const;
	const TextCoord& getTextCoord(const IndexGroup& pIndex) const;

	VertexList mVertices;
	NormalList mNormals;
	TextList mTextureCoords;
	FaceList mFaces;

	Material mMaterial;
};

#endif