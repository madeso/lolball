#ifndef RANDOM_HPP
#define RANDOM_HPP

// Mersenne Twister Pseudorandom number generator
// originally developed by Takuji Nishimura and Makoto Mtsumoto
// grabbed from Game Coding Complete by Mike McShaffry p85-87

// period parameters
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df // constant vector
#define CMATH_UPPER_MASK 0x80000000 // most significant w-r bits
#define CMATH_LOWER_MASK 0x7fffffff // least significant r bits

// tempering parameters
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y) (y>>11)
#define CMATH_TEMPERING_SHIFT_S(y) (y<<7)
#define CMATH_TEMPERING_SHIFT_T(y) (y<<15)
#define CMATH_TEMPERING_SHIFT_L(y) (y>>18)

class Random {
public:
	Random();

	//returns a number from 0 to n, excluding n
	unsigned long random(unsigned long n);
	unsigned long random();
	int randomSign();
	float randomFloat();
	float randomFloatWithSign();
	double randomDouble();
	bool randomBool();
	void setRandomSeed(unsigned long n);
	unsigned long getRandomSeed();
	void randomize();
private:
	unsigned long rseed;
	unsigned long mt[CMATH_N]; // the array for the state vector
	int mti; // mti=N+1 means mt[n] is not initialized
};


#endif // RANDOM_HPP