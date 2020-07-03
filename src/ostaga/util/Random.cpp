
// Precompiled
#include "ospch.h"
//

#include "Random.h"

#define IEEE_754_MANTISSA 0x007fffff
#define IEEE_754_ONE 0x3f800000

namespace Ostaga {

	unsigned int Random::s_Seed = 1u;

	unsigned int Random::Integer()
	{
		unsigned int random = Hash(s_Seed);
		s_Seed = random;
		return random;
	}

	float Random::Float()
	{
		unsigned int random = Integer();

		FloatBitsToInt bits;
		bits.IntBits = (random & IEEE_754_MANTISSA) | IEEE_754_ONE;

		return bits.FloatBits - 1.0f;
	}

	unsigned int Random::Hash(unsigned int value)
	{
		unsigned int hash = 0;
		hash += value;
		hash += hash << 10;
		hash ^= hash >> 6;
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}

}