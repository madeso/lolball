#ifndef SPOT_LIGHT_DATA_HPP
#define SPOT_LIGHT_DATA_HPP

#include "vec3.hpp"
#include "CommonLightAttributes.hpp"

struct SpotLightData : public CommonLightAttributes {
	SpotLightData();
	vec3 position;
	vec3 direction;

	float exponent;
	float cutoff;
};

#endif