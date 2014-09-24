#include "vec2.hpp"
#include <cmath>

const bool vec2::operator==(const vec2& vec) const {
	return math::equal(getX(), vec.getX()) && math::equal(getY(), vec.getY());
}

real vec2::getLength() const {
	return std::sqrtf(getLengthSquared());
}