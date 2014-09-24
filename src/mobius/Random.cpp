#include "Random.hpp"
#include <ctime>
#include <cassert>
#include <limits>

Random::Random() : rseed(1), mti(CMATH_N+1) {
}

unsigned long Random::random(unsigned long n) {
	assert(n!=0);
	return random() % n;
}

int Random::randomSign() {
	if( randomBool() ) return 1;
	return -1;
}
float Random::randomFloatWithSign() {
	return randomFloat() * randomSign();
}

float Random::randomFloat() {
	std::numeric_limits<unsigned long> lim;
	return (float) random() / (float) lim.max();
}
double Random::randomDouble() {
	std::numeric_limits<unsigned long> lim;
	return (double) random() / (double) lim.max();
}
bool Random::randomBool() {
	return random(2)==0;
}

unsigned long Random::random() {
	unsigned long y;
	static unsigned long mag01[2]={0x0, CMATH_MATRIX_A};
	// mag01[x] = x*MATRIX_a for x=0,1

	if( mti >= CMATH_N ) { // generate n words at one time
		int kk;
		if( mti == CMATH_N+1) { // if sgenrand has not been called
			setRandomSeed(4357);  // a default initial seed is used
		}

		for(kk=0; kk<CMATH_N-CMATH_M; ++kk) {
			y = ( mt[kk]&CMATH_UPPER_MASK) | (mt[kk+1]&CMATH_LOWER_MASK);
			mt[kk] = mt[kk+CMATH_M] ^ (y>>1) ^ mag01[y&0x1];
		}
		for(; kk<CMATH_N-1; ++kk) {
			y = (mt[kk]&CMATH_UPPER_MASK) | ( mt[kk+1]&CMATH_LOWER_MASK );
			mt[kk] = mt[kk+(CMATH_M-CMATH_N)] ^ (y>>1) ^ mag01[y&01];
		}
		y = (mt[CMATH_N-1]&CMATH_UPPER_MASK) | (mt[0]&CMATH_LOWER_MASK);
		mt[CMATH_N-1] = mt[CMATH_M-1] ^ (y>>1) ^ mag01[y&0x1];

		mti = 0;
	}

	y = mt[mti++];
	y ^= CMATH_TEMPERING_SHIFT_U(y);
	y ^= CMATH_TEMPERING_SHIFT_S(y) & CMATH_TEMPERING_MASK_B;
	y ^= CMATH_TEMPERING_SHIFT_T(y) & CMATH_TEMPERING_MASK_C;
	y ^= CMATH_TEMPERING_SHIFT_L(y);

	return y;
}

void
Random::setRandomSeed(unsigned long n) {
	/* 
	 * setting initial seed to mt[N] using
	 * the generator line 25 of table 1 in
	 * [KNUTH 1981], The Art of Computer Programming
	 *   Vol. 2 (2nd ed.) pp102]
	 */
	mt[0] = n & 0xffffffff;
	for(mti=1; mti<CMATH_N; ++mti) {
		mt[mti] = ( 69069 * mt[mti-1] ) & 0xffffffff;
	}
	rseed = n;
}

unsigned long
Random::getRandomSeed() {
	return rseed;
}

void
Random::randomize() {
	setRandomSeed(time(NULL));
}