#include "vec3.hpp"
#include <cmath>
#include <cassert>

const bool vec3::operator==(const vec3& vec) const {
	return math::equal(getX(), vec.getX()) && math::equal(getY(), vec.getY()) && math::equal(getZ(), vec.getZ());
}

real vec3::getLength() const {
	return sqrtf(getLengthSquared());
}
bool vec3::isUnit() const {
	return math::equal(1.0, getLengthSquared());
}
bool vec3::isZero() const {
	return math::equal(0.0, getLengthSquared());
}
void vec3::normalize() {
	const real len = getLength();
	if( math::equal(len, 0.0f) ) return;
	(*this) /= len;
}

namespace op {
	real vec3::lengthBetween(const ::vec3& from, const ::vec3& to) {
		return (from-to).getLength();
	}
	real vec3::lengthBetweenSquared(const ::vec3& from, const ::vec3& to) {
		return (from-to).getLengthSquared();
	}
	::vec3 vec3::getLinearInterpolation(const ::vec3& from, real value, const ::vec3& to) {
		return (to-from)*value + from;
	}
	void vec3::getLinearInterpolation(const ::vec3& from, real value, const ::vec3& to, ::vec3* out) {
		assert(out);
		*out = getLinearInterpolation(from, value, to);
	}

	real vec3::getCosAngleBetween(const ::vec3& a, const ::vec3& b) {
		assert(a.isUnit());
		assert(b.isUnit());
		return a dot b;
	}
	Angle vec3::getAngleBetween(const ::vec3& a, const ::vec3& b) {
		return Angle(acos(getCosAngleBetween(a, b)));
	}

	const ::vec3 vec3::origo(0,0,0);
	const ::vec3 vec3::xAxisPositive(1,0,0);
	const ::vec3 vec3::yAxisPositive(0,1,0);
	const ::vec3 vec3::zAxisPositive(0,0,1);
	const ::vec3 vec3::xAxisNegative(-1,0,0);
	const ::vec3 vec3::yAxisNegative(0,-1,0);
	const ::vec3 vec3::zAxisNegative(0, 0,-1);
};
