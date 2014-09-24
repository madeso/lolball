#ifndef ABSTRACT_MATERIAL_HPP
#define ABSTRACT_MATERIAL_HPP

#include "vec3.hpp"
#include <string>

struct AbstractMaterial {
	AbstractMaterial();

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;

	float alpha;
	float shininess;
	std::string texture;
};

#endif