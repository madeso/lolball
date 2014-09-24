#ifndef MATH_HPP
#define MATH_HPP

typedef float real;

namespace math {
		const real radToDeg(const real rad);
		const real degToRad(const real deg);

		const real PI = 3.1415926535898f;
		const real TWO_PI = PI * 2.0f;
		//const real PI = 3.14159265358979323846f; // grabbed from a source - is it really needed
		const real ONE_DEG_IN_RADIAN = PI / 180;

		const bool logicXor(const bool a, const bool b);

		const float abs(const float a);
		const float max(const float a, const float b);
		const float min(const float a, const float b);

		const float floor(const float a);
		const float ceil(const float a);

		const bool equal(const float a, const float b);
		const bool equal0(const float a, const float b);
		const bool equal1(const float a, const float b);
		const bool equal2(const float a, const float b);
		const bool equal3(const float a, const float b);
		const bool equal4(const float a, const float b);
		const bool equal5(const float a, const float b);
		const bool equal6(const float a, const float b);
		const bool equal7(const float a, const float b);
		
		const float clampZero(const float a);
		const float limitRange(const float min, const float a, const float max);

		const float wrapRange(const float min, const float a, const float max);

		const float square(const float a);
		const float cube(const float a);
		const float squareRoot(const float a);

		const float sin(const float a);
		const float cos(const float a);
		const float tan(const float a);

		const float asin(const float a);
		const float acos(const float a);
		const float atan(const float a);

		const float interpolate(const float p_from, float p_fromOrTo, float p_to);
}

#endif