#ifndef POINT_LIGHT_DATA_HPP
#define POINT_LIGHT_DATA_HPP

#include "vec3.hpp"
#include "CommonLightAttributes.hpp"

struct PointLightData : public CommonLightAttributes {
	PointLightData();
	vec3 position;
};

#endif