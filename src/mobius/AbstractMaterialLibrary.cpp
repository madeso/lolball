#include "AbstractMaterialLibrary.hpp"
#include "Exceptions.hpp"
#include <cassert>
#include "StringUtils.hpp"
#include "File.hpp"
#include "boost/lexical_cast.hpp"

AbstractMaterialLibrary::AbstractMaterialLibrary() {
}

void error_NoMaterialSet(const std::string& pFileName) {
	throw BadDataException("No material set when setting mat property in " + pFileName);
}

void AbstractMaterialLibrary::load(const std::string& pFileName) {
	std::istringstream file;
	mMaterials.clear();
	loadFileIntoString(pFileName, &file);
	std::string line;
	AbstractMaterial* material = 0;
	while( std::getline(file, line) ) {
		std::vector<std::string> splittedLine;
		splitString(" ", line, &splittedLine);
		const std::string id = splittedLine[0];

		if( id=="newmtl") {
			const std::string name = trim(splittedLine[1]);
			if( hasMaterial(name) ) {
				throw BadDataException( "Double material in " + pFileName );
			}
			material = newMaterial(name);
		}
		else if (id=="Ka") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			float r = boost::lexical_cast<float>(trim( splittedLine[1] ));
			float g = boost::lexical_cast<float>(trim( splittedLine[2] ));
			float b = boost::lexical_cast<float>(trim( splittedLine[3] ));
			material->ambient = vec3(r, g, b);
		}
		else if (id=="Ke") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			float r = boost::lexical_cast<float>(trim( splittedLine[1] ));
			float g = boost::lexical_cast<float>(trim( splittedLine[2] ));
			float b = boost::lexical_cast<float>(trim( splittedLine[3] ));
			material->emissive = vec3(r, g, b);
		}
		else if (id=="Kd") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			float r = boost::lexical_cast<float>(trim( splittedLine[1] ));
			float g = boost::lexical_cast<float>(trim( splittedLine[2] ));
			float b = boost::lexical_cast<float>(trim( splittedLine[3] ));
			material->diffuse = vec3(r, g, b);
		}
		else if (id=="Ks") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			float r = boost::lexical_cast<float>(trim( splittedLine[1] ));
			float g = boost::lexical_cast<float>(trim( splittedLine[2] ));
			float b = boost::lexical_cast<float>(trim( splittedLine[3] ));
			material->specular = vec3(r, g, b);
		}
		else if (id=="d") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			material->alpha = boost::lexical_cast<float>(trim( splittedLine[1] ));
		}
		else if (id=="Tr") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			material->alpha = boost::lexical_cast<float>(trim( splittedLine[1] ));
		}
		else if (id=="Ns") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			material->shininess = boost::lexical_cast<float>(trim( splittedLine[1] ));
		}
		else if (id=="map_Ka") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			material->texture = trim( splittedLine[1] );
		}
		else if (id=="map_Kd") {
			if( material == 0 ) error_NoMaterialSet(pFileName);
			material->texture = trim( splittedLine[1] );
		}
	}
}
const AbstractMaterial& AbstractMaterialLibrary::getMaterial(const std::string& pName) const {
	StringMaterialMap::const_iterator result = mMaterials.find(pName);
	assert ( result != mMaterials.end() );
	return result->second;
}
bool AbstractMaterialLibrary::hasMaterial(const std::string& pName) const {
	StringMaterialMap::const_iterator result = mMaterials.find(pName);
	return ( result != mMaterials.end() );
}
AbstractMaterial* AbstractMaterialLibrary::newMaterial(const std::string& pName) {
	assert( !hasMaterial(pName) );
	mMaterials.insert( StringMaterialPair(pName, AbstractMaterial() ) );
	StringMaterialMap::iterator result = mMaterials.find(pName);
	assert ( result != mMaterials.end() );
	return &(result->second);
}