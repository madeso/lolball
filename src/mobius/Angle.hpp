#ifndef ANGLE_HPP
#define ANGLE_HPP

#include "math.hpp"

class Angle {
public:
	explicit Angle(real radians) : mRadians(radians) {}
	Angle(real degrees, bool unused);
	real inRadians() const ;
	real inDegrees() const ;
private:
	const real mRadians;
};

#endif