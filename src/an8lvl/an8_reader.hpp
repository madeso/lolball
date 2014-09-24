#ifndef __AN8_READER_HPP
#define __AN8_READER_HPP

#include <list>
#include <vector>
#include <map>

typedef float Real;

class Chunk;
typedef std::multimap<std::string, Chunk*>::const_iterator ChunksResult;
typedef std::multimap<std::string, Chunk*> ChunkMap;
typedef std::pair<std::string, Chunk*> ChunkEntry;

class Ident {
public:
	Ident();
	void setName(const std::string& pName);
	const std::string& getName() const;
private:
	std::string mName;
};

class String {
public:
	String();
	void setValue(const std::string& pValue);
	const std::string& getValue();
private:
	std::string mValue;
};

enum DATATYPE {
	DT_STRING,
	DT_REAL,
	DT_INT,
	DT_CONSTANT
};

class StringData {
public:
	void setValue(const std::string& pValue, const bool pString);
	const std::string& getValue() const;
	const DATATYPE determineType() const;
private:
	std::string mValue;
	bool mString;
};

enum CHUNK_CHILD_TYPE {
	CCT_CHILD,
	CCT_INT,
	CCT_REAL,
	CCT_STRING,
	CCT_COMPOUND,
	CCT_CONSTANT
};

class Child {
public:
	virtual ~Child() {}
	virtual CHUNK_CHILD_TYPE getType() const = 0;
};

class ChildList {
public:
	ChildList();
	~ChildList();
	void add(Child* pChild);
	unsigned long count() const;
	const Child* childAt(unsigned long pIndex) const;
	ChunksResult end() const;

	ChunksResult get(const std::string& pItemName) const;
	const Chunk* at(const std::string& pItemName) const;
	const Chunk* at(const std::string& pItemName, const std::string& pObjectName, const bool pAcceptNoChunk) const;
	bool has(const std::string& pItemName) const;
private:
	typedef std::vector<Child*> ChildContainer;
	ChildContainer mChilds;
	ChunkMap mChunks; // for faster reference
};
class ValueChild : public Child {
public:
	static ValueChild* createChild(const StringData& data);
	virtual std::string value() const =0; // return the value and checks if it can be converted to a string
	virtual std::string asString() const = 0 ; // converts the value to a string
};
bool isValue(CHUNK_CHILD_TYPE cct);
	
class CompoundChild : public ValueChild {
public:
	CompoundChild();
	CHUNK_CHILD_TYPE getType() const;
	~CompoundChild();
	std::string value() const;
	std::string asString() const;
	void addValue(ValueChild* pValue);
	unsigned long count() const;
	const ValueChild* at(const unsigned long index) const;
private:
	typedef std::vector<ValueChild*> ValueList;
	ValueList mValues;
};
class StringChild : public ValueChild {
public:
	StringChild(const StringData& p_data);
	const std::string& getValue() const;
	CHUNK_CHILD_TYPE getType() const;
	std::string value() const;
	std::string asString() const ;
private:
	const std::string mString;
};
class ConstantChild : public ValueChild {
public:
	ConstantChild(const StringData& p_data);
	const std::string& getValue() const;
	CHUNK_CHILD_TYPE getType() const;
	std::string value() const;
	std::string asString() const;
private:
	const std::string mConst;
};
class NumberChild : public ValueChild {
public:
	virtual const Real getValueAsReal() const =0;
	virtual const int getValueAsInt() const = 0;
};
bool isReal(CHUNK_CHILD_TYPE cct);
class IntChild : public NumberChild {
public:
	IntChild(const StringData& p_data);
	CHUNK_CHILD_TYPE getType() const ;
	const Real getValueAsReal() const;
	const int getValueAsInt() const;
	std::string value() const ;
	std::string asString() const ;
private:
	int mInt;
};
class RealChild : public NumberChild {
public:
	RealChild(const StringData& p_data);
	CHUNK_CHILD_TYPE getType() const ;
	const Real getValueAsReal() const ;
	const int getValueAsInt() const;
	std::string value() const ;
	std::string asString() const ;
private:
	Real mReal;
};

class Chunk {
public:
	Chunk();
	void setName(const Ident& pName);
	const Ident& getName() const ;
	const bool hasName() const;
	void addChild(Child* pChild);
	const ChildList& getChildList() const;
private:
	ChunkMap mChunks;
	Ident mName;
	bool mHasName;
	ChildList mChildren;
};

class ChunkList {
public:
	ChunkList();
	~ChunkList();
	void addChunk(Chunk* p_chunk);
	ChunksResult get(const std::string& pItemName) const;
	ChunksResult end() const;
	const Chunk* at(const std::string& pItemName) const ;
	bool has(const std::string& pItemName) const;
	const Chunk* at(const std::string& pItemName, const std::string& pObjectName, const bool pAcceptNoChunk) const;
private:
	ChunkMap mChunks;
};

void test(std::ifstream& ifs);
enum SPACESKIP {
	SS_SPACE,
	SS_SLASH,
	SS_COMMENT,
	SS_STAR
};
char skipSpace(std::ifstream& ifs);

std::ifstream& operator>> (std::ifstream& ifs, Ident& pIndent);

enum State {
	ST_STRING,
	ST_REAL,
	ST_CONSTANT,
	ST_UNKNOWN
};

std::ifstream& operator>> (std::ifstream& ifs, StringData& pString);

class ChunkChild : public Child {
public:
	ChunkChild();
	CHUNK_CHILD_TYPE getType() const;
	void setName(const Ident& pName);
	void read(std::ifstream& ifs);
	Chunk* getChild();
	const Chunk* getChild() const;
private:
	Chunk mChunk;
};

CompoundChild* readCompoundStatement(std::ifstream& ifs);
std::ifstream& operator>>(std::ifstream& ifs, Chunk& p_chunk);

class ChunkReader {
public:
	static ChunkList* loadFile(std::string p_file);
};

const ValueChild* asValueChild(const Child* pChild);
const NumberChild* asRealChild(const Child* pChild);
const ChunkChild* asChunkChild(const Child* pChild);
const StringChild* asStringChild(const Child* pChild);

const CompoundChild* asCompoundChild(const Child* child);

Real getRealValue(const ValueChild* pChild);

enum FaceFlag {
	SF_SHOW_BACK=1,   //Don't backface cull this face.
    SF_HAS_NORMALS=2, //This face has normal data in this file.
    SF_HAS_TEXTURE=4, //This face has texture coordinates.
};



#endif // __AN8_READER_HPP