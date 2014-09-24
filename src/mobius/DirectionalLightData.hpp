#ifndef DIRECTIONAL_LIGHT_DATA_HPP
#define DIRECTIONAL_LIGHT_DATA_HPP

#include "vec3.hpp"
#include "CommonLightAttributes.hpp"

struct DirectionalLightData : public CommonLightAttributes {
	DirectionalLightData();
	vec3 direction;
};

#endif