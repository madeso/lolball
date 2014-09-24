#include "Angle.hpp"

Angle::Angle(real degrees, bool unused) : mRadians(math::degToRad(degrees)) {
}

real Angle::inRadians() const {
	return mRadians;
}
real Angle::inDegrees() const {
	return math::radToDeg(mRadians);
}