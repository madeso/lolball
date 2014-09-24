#ifndef ABSTRACT_MATERIAL_LIBRARY_HPP
#define ABSTRACT_MATERIAL_LIBRARY_HPP

#include "AbstractMaterial.hpp"
#include <map>
#include <string>

class AbstractMaterialLibrary {
public:
	AbstractMaterialLibrary();

	void load(const std::string& pFileName);
	const AbstractMaterial& getMaterial(const std::string& pName) const;
private:
	bool hasMaterial(const std::string& pName) const;
	AbstractMaterial* newMaterial(const std::string& pName);
	
	typedef std::map<std::string, AbstractMaterial> StringMaterialMap;
	typedef std::pair<std::string, AbstractMaterial> StringMaterialPair;

	StringMaterialMap mMaterials;
};

#endif