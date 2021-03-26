#include "Random.h"

Random::Random()
{
	srand(rd()); magicNumber = rand();
}

uint32_t Random::myRand()
{
	magicNumber += 0xe120fc15;
	uint64_t tmp;
	tmp = (uint64_t)magicNumber * 0x4a39b70d;
	uint32_t m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (tmp >> 32) ^ tmp;
	return m2;
}

Pixel Random::GetSimilarColor(Pixel color, int modulus)
{
	int biggestValue = std::max({ color.r, color.g, color.b });
	int redMod = biggestValue == color.r ? -((int)myRand() % modulus) : myRand() % modulus;
	int greenMod = biggestValue == color.g ? -((int)myRand() % modulus) : myRand() % modulus;
	int blueMod = biggestValue == color.b ? -((int)myRand() % modulus) : myRand() % modulus;
	Pixel value =
	{
		(uint8_t)(color.r + redMod),
		(uint8_t)(color.g + greenMod),
		(uint8_t)(color.b + blueMod),
		color.a
	};
	return value;
}
