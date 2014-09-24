#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <memory>

#include "an8_reader.hpp"


using namespace std;
#define LMINFO(x) cout << x << endl;
#define LINFO(x) cout << x << endl;

bool isSpace(char c) {
	switch(c) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;
		default:
			return false;
	}
}
bool isIdentChar(char c, bool first) {
	if( !first ) {
		switch(c) {
			case '0': case '1': case '2':
			case '3': case '4': case '5':
			case '6': case '7': case '8':
			case '9':
				return true;
		}
	}
	switch(c) {
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': 
		case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': 
		case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': 
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': 
		case 'y': case 'z': 

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': 
		case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': 
		case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': 
		case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
		case 'Y': case 'Z':

		case '_': 
			return true;
	}
	return false;
}

bool isRealChar(char c, bool first) {
	if( first ) {
		switch(c) {
			case '+':
			case '-':
				return true;
		}
	}
	switch(c) {
		case '0': case '1': case '2':
		case '3': case '4': case '5':
		case '6': case '7': case '8':
		case '9':
			return true;
		default:
			return false;
	}
}
bool isSign(char c) {
	switch(c) {
		case '+': case '-':
			return true;
		default:
			return false;
	}
}


Ident::Ident() {
}
void Ident::setName(const std::string& pName) {
	mName = pName;
}
const std::string& Ident::getName() const {
	return mName;
}

String::String() {
}
void String::setValue(const std::string& pValue) {
	mValue = pValue;
}
const std::string& String::getValue() {
	return mValue;
}


void StringData::setValue(const std::string& pValue, const bool pString) {
	mValue = pValue;
	mString = pString;
}
const std::string& StringData::getValue() const {
	return mValue;
}
const DATATYPE StringData::determineType() const {
	if( mString ) return DT_STRING;
	if( mValue.empty() ) return DT_STRING; // empty string is considered string
	if( isRealChar(mValue[0], true) ) {
		if( mValue.find('.') == std::string::npos ) {
			return DT_INT;
		}
		else {
			return DT_REAL;
		}
	}
	if( isIdentChar(mValue[0], true) ) return DT_CONSTANT;
	return DT_STRING; // consider it a string anyway
}


ChildList::ChildList() {
}
ChildList::~ChildList() {
	for(ChildContainer::iterator current=mChilds.begin(); current != mChilds.end(); ++current) {
		Child* deleteMe = *current;
		if( deleteMe ) {
			delete deleteMe;
			deleteMe = 0;
		}
	}
}

unsigned long ChildList::count() const {
	return mChilds.size();
}
const Child* ChildList::childAt(unsigned long pIndex) const {
	return mChilds.at(pIndex);
}
ChunksResult ChildList::end() const {
	return mChunks.end();
}

ChunksResult ChildList::get(const std::string& pItemName) const {
	ChunksResult res = mChunks.find(pItemName);
	if( res==mChunks.end() ){
		std::stringstream errorMessage;
		errorMessage << "Failed to find item: " << pItemName;
		throw errorMessage.str();
	}
	return res;
}
const Chunk* ChildList::at(const std::string& pItemName) const {
	ChunksResult res = get(pItemName);
	return (*res).second;
}
bool ChildList::has(const std::string& pItemName) const {
	const ChunksResult res = mChunks.find(pItemName);
	if( res==mChunks.end() ) return false;
	return true;
}

bool isValue(CHUNK_CHILD_TYPE cct) {
	switch(cct) {
	case CCT_CHILD:
	case CCT_COMPOUND:
		return false;
	case CCT_INT:
	case CCT_REAL:
	case CCT_STRING:
	case CCT_CONSTANT:
		return true;
	default:
		return false;
	}
}


CompoundChild::CompoundChild() {
}
CHUNK_CHILD_TYPE CompoundChild::getType() const {
	return CCT_COMPOUND;
}
CompoundChild::~CompoundChild() {
	for(ValueList::iterator current = mValues.begin(); current!=mValues.end(); ++current) {
		ValueChild* child = *current;
		if( child ) {
			delete child;
			child = 0;
		}
	}
}
std::string CompoundChild::value() const {
	throw std::string("Unable to convert to string");
	return "";
}
std::string CompoundChild::asString() const {
	return "";
}
void CompoundChild::addValue(ValueChild* pValue) {
	mValues.push_back(pValue);
}
unsigned long CompoundChild::count() const {
	return mValues.size();
}
const ValueChild* CompoundChild::at(const unsigned long index) const {
	return mValues[index];
}


StringChild::StringChild(const StringData& p_data) : mString(p_data.getValue()) {
}
const std::string& StringChild::getValue() const {
	return mString;
}
CHUNK_CHILD_TYPE StringChild::getType() const {
	return CCT_STRING;
}
std::string StringChild::value() const {
	return mString;
}
std::string StringChild::asString() const {
	return mString;
}


ConstantChild::ConstantChild(const StringData& p_data) : mConst(p_data.getValue()) {
}
const std::string& ConstantChild::getValue() const {
	return mConst;
}
CHUNK_CHILD_TYPE ConstantChild::getType() const {
	return CCT_CONSTANT;
}
std::string ConstantChild::value() const {
	throw std::string("Unable to convert to string");
	return "";
}
std::string ConstantChild::asString() const {
	return mConst;
}


bool isReal(CHUNK_CHILD_TYPE cct) {
	switch(cct) {
	case CCT_CHILD:
	case CCT_COMPOUND:
	case CCT_STRING:
	case CCT_CONSTANT:
		return false;
	case CCT_INT:
	case CCT_REAL:
		return true;
	default:
		return false;
	}
}

IntChild::IntChild(const StringData& p_data) : mInt(0) {
	std::stringstream data(p_data.getValue());
	data >> mInt;
}
CHUNK_CHILD_TYPE IntChild::getType() const {
	return CCT_INT;
}
const Real IntChild::getValueAsReal() const {
	return (Real) mInt;
}
const int IntChild::getValueAsInt() const {
	return mInt;
}
std::string IntChild::value() const {
	std::stringstream stream;
	stream << mInt;
	return stream.str();
}
std::string IntChild::asString() const {
	std::stringstream stream;
	stream << mInt;
	return stream.str();
}


RealChild::RealChild(const StringData& p_data) : mReal(0.0) {
	std::stringstream data(p_data.getValue());
	data >> mReal;
}
CHUNK_CHILD_TYPE RealChild::getType() const {
	return CCT_REAL;
}
const Real RealChild::getValueAsReal() const {
	return mReal;
}
const int RealChild::getValueAsInt() const {
	return (int) mReal;
}
std::string RealChild::value() const {
	std::stringstream stream;
	stream << mReal;
	return stream.str();
}
std::string RealChild::asString() const {
	std::stringstream stream;
	stream << mReal;
	return stream.str();
}


Chunk::Chunk() : mHasName(false) {
}
void Chunk::setName(const Ident& pName) {
	mName = pName;
	mHasName = true;
}
const Ident& Chunk::getName() const {
	return mName;
}
const bool Chunk::hasName() const {
	return mHasName;
}
void Chunk::addChild(Child* pChild) {
	assert(pChild);
	mChildren.add(pChild);
}
const ChildList& Chunk::getChildList() const {
	return mChildren;
}


ChunkList::ChunkList() {
}
ChunkList::~ChunkList() {
	for(ChunkMap::iterator current=mChunks.begin(); current!=mChunks.end(); ++current) {
		ChunkEntry entry = *current;
		if( entry.second ) {
			delete entry.second;
			entry.second = 0;
		}
	}
}
void ChunkList::addChunk(Chunk* p_chunk) {
	assert(p_chunk);
	ChunkEntry entry(p_chunk->getName().getName(), p_chunk);
	mChunks.insert(entry);
}
ChunksResult ChunkList::get(const std::string& pItemName) const {
	ChunksResult res = mChunks.find(pItemName);
	if( res==mChunks.end() ){
		std::stringstream errorMessage;
		errorMessage << "Failed to find item: " << pItemName;
		throw errorMessage.str();
	}
	return res;
}
ChunksResult ChunkList::end() const {
	return mChunks.end();
}
const Chunk* ChunkList::at(const std::string& pItemName) const {
	ChunksResult res = get(pItemName);
	return (*res).second;
}
bool ChunkList::has(const std::string& pItemName) const {
	const ChunksResult res = mChunks.find(pItemName);
	if( res==mChunks.end() ) return false;
	return true;
}

void test(std::ifstream& ifs) {
	if( ifs.eof() ) {
		throw std::string("EOF detected, this shouldnt happen");
	}
	if( !ifs.good() ) {
		throw std::string("Read error detected");
	}
}

char skipSpace(std::ifstream& ifs) {
	char charRead;
	if( ifs.eof() ) return 0;
	test(ifs);
	SPACESKIP state = SS_SPACE;
	bool done = false;
	while( !done && ifs.get(charRead) ) {
		switch(state) {
		case SS_SPACE:
			if( isSpace(charRead) ) {
				continue;
			}
			else if( charRead=='/' ) {
				state = SS_SLASH;
			}
			else {
				done = true;
				break;
			}
			break;
		case SS_SLASH:
			if( charRead=='*' ) {
				state = SS_COMMENT;
			}
			else {
				state = SS_SPACE;
				ifs.putback('/');
				done = true;
				break;
			}
			break;
		case SS_COMMENT:
			if( charRead=='*' ) {
				state = SS_STAR;
			}
			break;
		case SS_STAR:
			if( charRead=='/' ) {
				state = SS_SPACE;
			}
			break;
		}
	}
	if(ifs.eof() ) return 0;
	test(ifs);
	return charRead;
}

std::ifstream& operator>> (std::ifstream& ifs, Ident& pIndent) {
	if(ifs.eof() ) return ifs; test(ifs);
	std::stringstream identName;

	char charRead = skipSpace(ifs);
	if( ifs.eof() ) return ifs;
	if(!isIdentChar(charRead, true) ) {
		std::stringstream str;
		str << "failed to read ident, " << charRead << " is not a valid IDENT char";
		throw str.str();
	}

	identName << charRead;
	while( ifs.get(charRead) && isIdentChar(charRead, false) ) {
		identName << charRead;
	}
	if( !isIdentChar(charRead, false) ) {
		ifs.putback(charRead);
	}

	pIndent.setName(identName.str());

	return ifs;
}


std::ifstream& operator>> (std::ifstream& ifs, StringData& pString) {
	if(ifs.eof() ) return ifs; test(ifs);
	std::stringstream value;

	char charRead = skipSpace(ifs);
	if( ifs.eof() ) return ifs;
	State state = ST_STRING;
	if( charRead != '"' ) {
		if( isIdentChar(charRead, true) ) {
			state = ST_CONSTANT;
			value << charRead;
		}
		else if( isRealChar(charRead, true) ) {
			state = ST_REAL;
			value << charRead;
		}
		else if( isSign(charRead) ) {
			state = ST_REAL;
			value << charRead;
		}
		else {
			std::stringstream str;
			str << "Unknown character in VALUE: " << charRead;
			throw str.str();
		}
	}
	bool readComma=false;
	bool done=false;
	while( !done ) {
		ifs.get(charRead);
		if( ifs.eof() ) {
			done = true;
			break;
		}
		test(ifs);
		if( state != ST_STRING ) {
			// nonstrings may not contain theese
			switch(charRead) {
				case '{': case '}':
				case '(': case '"':
					done = true;
			}
		}
		if(done) break;
		switch( state ) {
			case ST_STRING:
				if( charRead =='\\' ) {
					char nextChar;
					ifs.get(nextChar);
					switch(nextChar) {
			case '\t': value << '\t'; break;
			case '\n': value << '\n'; break;
			default: value << nextChar; break;
					}
					continue;
				}
				else if( charRead=='"') {
					done=true;
				}
				else {
					value << charRead;
				}
				break;
			case ST_REAL:
				if( isRealChar(charRead, false) ) {
					value << charRead;
				}
				else {
					if( charRead=='.' ) {
						if( readComma ) {
							throw std::string("Real error: Read comma twice");
						}
						else {
							readComma = true;
							value << charRead;
						}
					}
					else {
						done = true;
					}
				}
				break;
			case ST_CONSTANT:
				if( isIdentChar(charRead, false) ) {
					value << charRead;
				}
				else {
					done = true;
				}
				break;
			default:
				throw std::string("state error");
		}
	}
	if( state != ST_STRING ) {
		// dont put back the latest char of a string ( ie the ")
		ifs.putback(charRead);
	}

	pString.setValue(value.str(), state==ST_STRING);

	return ifs;
}


ChunkChild::ChunkChild() {
}
CHUNK_CHILD_TYPE ChunkChild::getType() const {
	return CCT_CHILD;
}
void ChunkChild::setName(const Ident& pName) {
	mChunk.setName(pName);
}
void ChunkChild::read(std::ifstream& ifs) {
	ifs >> mChunk;
}
Chunk* ChunkChild::getChild() {
	return &mChunk;
}
const Chunk* ChunkChild::getChild() const {
	return &mChunk;
}

ValueChild* ValueChild::createChild(const StringData& data) {
	switch(data.determineType()) {
		case DT_STRING:
			return new StringChild(data);
			break;
		case DT_REAL:
			return new RealChild(data);
			break;
		case DT_INT:
			return new IntChild(data);
			break;
		case DT_CONSTANT:
			return new ConstantChild(data);
			break;
		default:
			throw std::string("Error in code found in Child::createChild() while reading an8");
	}
	return 0;
}

/** @todo add safer convertion here */
void ChildList::add(Child* pChild) {
	if( pChild->getType() == CCT_CHILD ) {
		ChunkChild* chunkChild = (ChunkChild*) pChild;
		ChunkEntry entry(chunkChild->getChild()->getName().getName(), chunkChild->getChild());
		mChunks.insert(entry);
	}
	mChilds.push_back(pChild);
}

CompoundChild* readCompoundStatement(std::ifstream& ifs) {
	char charRead = skipSpace(ifs);
	if( ifs.eof() ) return 0;
	if( charRead != '(' ) {
		throw std::string("Couldnt find ( when starting to read compound statement, this shouldnt happen");
	}
	std::auto_ptr<CompoundChild> child;
	child.reset( new CompoundChild() );
	bool done = false;
	while(!done) {
		char testChar = skipSpace(ifs);
		if( ifs.eof() ) return 0;
		if( testChar == ')' ) {
			done = true;
			break;
		}
		else if (testChar=='(' ) {
			ifs.putback(testChar);
			std::auto_ptr<CompoundChild> subChild;
			subChild.reset(readCompoundStatement(ifs));
			if(ifs.eof() ) throw std::string("Detected eof when reading sub compound child in compound child");
			test(ifs);
			child->addValue( subChild.release() );
		}
		else {
			ifs.putback(testChar);
			// string data
			StringData string;
			ifs >> string;
			if(ifs.eof() ) throw std::string("Detected eof when reading data in compound child"); test(ifs);
			ValueChild* childData = ValueChild::createChild(string);
			if( childData ) {
				child->addValue(childData);
			}
			else {
				throw std::string("Error in creation process of the data.");
			}
		}
	}
	return child.release();
}

std::ifstream& operator>>(std::ifstream& ifs, Chunk& p_chunk) {
	if(ifs.eof() ) return ifs;
	test(ifs);
	if( !p_chunk.hasName() ) {
		Ident name;
		ifs >> name;
		if(ifs.eof() ) return ifs;
		test(ifs);
		p_chunk.setName(name);
	}
	char charRead = skipSpace(ifs);
	if(ifs.eof() ) throw std::string("Detected eof before start {");
	if( charRead == '{' ) {
		bool done=false;
		while(!done) {
			charRead = skipSpace(ifs);
			if(ifs.eof() ) throw std::string("Detected eof before end }");
			if( charRead =='}' ) {
				done = true;
			}
			else {
				ifs.putback(charRead);
				char testChar = skipSpace(ifs);
				if(ifs.eof() ) throw std::string("Detected eof before end }"); test(ifs);
				ifs.putback(testChar);
				if( testChar == '(' ) {
					// compound statement
					std::auto_ptr<CompoundChild> child;
					child.reset(readCompoundStatement(ifs));
					if(ifs.eof() ) throw std::string("Detected eof while reading compound");
					test(ifs);
					p_chunk.addChild( child.release() );
					continue;
				}

				// read string data or chunk(constant followed by a {)
				StringData string;
				ifs >> string;
				if(ifs.eof() ) throw std::string("Detected eof before end }"); test(ifs);
				// add another chunks
				testChar = skipSpace(ifs);
				if(ifs.eof() ) throw std::string("Detected eof before end }"); test(ifs);
				if( testChar == '{' ) {
					ifs.putback('{');
					std::auto_ptr<ChunkChild> subchunk;
					// prevent non constants followed by a {
					if( string.determineType() != DT_CONSTANT ) {
						throw std::string("data type followed by a {");
					}
					Ident name; name.setName(string.getValue());
					subchunk.reset(new ChunkChild());
					subchunk->setName( name );

					subchunk->read(ifs);
					if( ifs.eof() ) {
						throw std::string("eof detected while reading sub-chunk");
					}
					p_chunk.addChild( subchunk.release() );
				}
				else {
					ifs.putback(testChar);
					ValueChild* child = ValueChild::createChild(string);
					if( child ) {
						p_chunk.addChild(child);
					}
					else {
						throw std::string("Error in creation process of the data.");
					}
				}

			}
		}
	}
	else {
	}
	return ifs;
}

ChunkList* ChunkReader::loadFile(std::string p_file) {
	std::auto_ptr<ChunkList> chunkList;
	std::ifstream ifs (p_file.c_str());
	if( !ifs.good() ) {
		string message = "Failed to properly load ";
		message += p_file;
		throw message;
	}
	chunkList.reset( new ChunkList() );
	while( !ifs.eof() ) {
		std::auto_ptr<Chunk> chunk;
		chunk.reset(new Chunk());
		ifs >> (*chunk); // read data
		// check chunk if it is valid here
		chunkList->addChunk(chunk.release());
	}
	return chunkList.release();
}

const ValueChild* asValueChild(const Child* pChild) {
	if( isValue(pChild->getType())) {
		return (const ValueChild*) pChild;
	}
	throw std::string("requested child is not a value type");
	return 0;
}
const NumberChild* asRealChild(const Child* pChild) {
	if( isReal(pChild->getType()) ) {
		return (const NumberChild*) pChild;
	}
	throw std::string("requested child is not a Real type");
	return 0;
}
const ChunkChild* asChunkChild(const Child* pChild) {
	if( pChild->getType() == CCT_CHILD ) {
		return (const ChunkChild*) pChild;
	}
	throw std::string("requested child is not a child type");
	return 0;
}
const Chunk* ChildList::at(const std::string& pItemName, const std::string& pObjectName, const bool pAcceptNoChunk) const {
	for(ChunksResult current= mChunks.find(pItemName); current!=mChunks.end(); ++current ) {
		const Chunk* chunk = (*current).second;
		if( asValueChild(chunk->getChildList().childAt(0))->value()==pObjectName ) {
			return chunk;
		}
	}
	if( !pAcceptNoChunk ) {
		std::stringstream errorReport;
		errorReport << "No "<< pItemName << " named " << pObjectName <<  "found!";
		throw std::string("no object found");
	}
	return 0;
}
const Chunk* ChunkList::at(const std::string& pItemName, const std::string& pObjectName, const bool pAcceptNoChunk) const {
	for(ChunksResult current= mChunks.find(pItemName); current!=mChunks.end(); ++current ) {
		const Chunk* chunk = (*current).second;
		if( asValueChild(chunk->getChildList().childAt(0))->value()==pObjectName ) {
			return chunk;
		}
	}
	if( !pAcceptNoChunk ) {
		std::stringstream errorReport;
		errorReport << "No "<< pItemName << " named " << pObjectName <<  "found!";
		throw std::string("no object found");
	}
	return 0;
}

const CompoundChild* asCompoundChild(const Child* child) {
	if( child->getType() != CCT_COMPOUND ) {
		throw std::string("Expecting a compound statement, found something else");
	}
	return (const CompoundChild*) child;
}

const StringChild* asStringChild(const Child* pChild) {
	if( pChild->getType() != CCT_STRING ) {
		throw std::string("Expecting a string statement, found something else");
	}
	return (const StringChild*) pChild;
}

Real getRealValue(const ValueChild* pChild) {
	if( isReal( pChild->getType() ) ) {
		const NumberChild* c = (const NumberChild*) pChild;
		return c->getValueAsReal();
	}
	else{
		throw std::string("Failed to convert child to a real");
		return 0.0f;
	}
}