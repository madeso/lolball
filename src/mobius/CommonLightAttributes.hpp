#ifndef COMMON_LIGHT_ATTRIBUTES
#define COMMON_LIGHT_ATTRIBUTES

#include "vec3.hpp"

struct Attenuation {
	Attenuation();

	float constant;
	float linear;
	float quadratic;
};

struct CommonLightAttributes {
	CommonLightAttributes();

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	Attenuation attenuation;
};

#endif