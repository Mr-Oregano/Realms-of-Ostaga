
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

	float Random::Float(float x)
	{
		FloatBitsToInt input;
		input.FloatBits = x;
		unsigned int random = Hash(input.IntBits, s_Seed);

		FloatBitsToInt bits;
		bits.IntBits = (random & IEEE_754_MANTISSA) | IEEE_754_ONE;

		return bits.FloatBits - 1.0f;
	}

	float Random::Float(float x, float y)
	{
		FloatBitsToInt input1;
		input1.FloatBits = x;

		FloatBitsToInt input2;
		input2.FloatBits = y;

		unsigned int random = Hash(input1.IntBits, input2.IntBits, s_Seed);

		FloatBitsToInt bits;
		bits.IntBits = (random & IEEE_754_MANTISSA) | IEEE_754_ONE;

		return bits.FloatBits - 1.0f;
	}

	float Random::Simplex(float x, float y)
	{
		glm::vec2 coords = { x, y };
		// Preset Gradients for each simplex vertex.
		static const glm::vec2 gradients2D[] = {
			{ 1.0, 1.0  },
			{ 1.0, -1.0 },
			{-1.0, -1.0 },
			{-1.0, 1.0  }
		};
		static const unsigned int gradients2D_length = (unsigned int) std::size(gradients2D);

		// Transform the simplex grid into tile-space in order
		// to extract the origin of the current hypercube.
		const float F2 = (glm::sqrt(3.0f) - 1.0f) / 2.0f;
		float s = (coords.x + coords.y) * F2;
		glm::vec2 tile = floor(coords + s);

		// Transform the previously calculated tile coorindate
		// back into x,y space and use it to calculate the displacement
		// vector from the origin vertex to the fragment coordinates.
		const float G2 = (1.0f - 1.0f / glm::sqrt(3.0f)) / 2.0f;
		float t = (tile.x + tile.y) * G2;
		glm::vec2 d0 = coords - (tile - t);

		// Determine which simplex the fragment coordinates is in.
		// If the displacement vector's 'x > y', then we are in bottom
		// simplex with middle coordinate (1, 0). Otherwise we are in 
		// simplex with middle coordinate of (0, 1).
		float simplex_loc = glm::abs(glm::step(0.0f, d0.x - d0.y));
		glm::vec2 middle_corner = glm::vec2(simplex_loc, 1.0f - simplex_loc);

		// Calculate remaining displacement vectors from the other
		// vertices to the fragment coordinates.
		glm::vec2 d1 = d0 - middle_corner + G2;
		glm::vec2 d2 = d0 - 1.0f + 2.0f * G2;

		// Psuedo-randomly select gradients for each of the three simplex vertices.
		glm::vec2 gradTile0 = tile + glm::vec2(0.0f, 0.0f);
		glm::vec2 gradTile1 = tile + middle_corner;
		glm::vec2 gradTile2 = tile + glm::vec2(1.0f, 1.0f);
		glm::vec2 grad0 = gradients2D[Hash(ConvertFloatBitsToInt(gradTile0.x), ConvertFloatBitsToInt(gradTile0.y), s_Seed) % gradients2D_length];
		glm::vec2 grad1 = gradients2D[Hash(ConvertFloatBitsToInt(gradTile1.x), ConvertFloatBitsToInt(gradTile1.y), s_Seed) % gradients2D_length];
		glm::vec2 grad2 = gradients2D[Hash(ConvertFloatBitsToInt(gradTile2.x), ConvertFloatBitsToInt(gradTile2.y), s_Seed) % gradients2D_length];

		// Calculate contributions from each of the 3 corners based off a multiplication
		// of a radially symmetric attenuation function and the gradient ramp calculated from the
		// scalar product of the displacement vectors and the previously selected gradients.
		float contribution0 = glm::pow(glm::max(0.0f, 0.5f - d0.x * d0.x - d0.y * d0.y), 4.0f) * dot(grad0, d0);
		float contribution1 = glm::pow(glm::max(0.0f, 0.5f - d1.x * d1.x - d1.y * d1.y), 4.0f) * dot(grad1, d1);
		float contribution2 = glm::pow(glm::max(0.0f, 0.5f - d2.x * d2.x - d2.y * d2.y), 4.0f) * dot(grad2, d2);

		// Sum the contributions and normalize the output between [0.0, 1.0)
		return (70.0f * (contribution0 + contribution1 + contribution2) + 1.0f) / 2.0f;
	}

	unsigned int Random::ConvertFloatBitsToInt(float f)
	{
		FloatBitsToInt ftoi;
		ftoi.FloatBits = f;
		return ftoi.IntBits;
	}

	unsigned int Random::Hash(unsigned int seed)
	{
		unsigned int hash = 0;
		hash += seed;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}

	unsigned int Random::Hash(unsigned int v1, unsigned int seed)
	{
		unsigned int hash = 0;
		hash += v1;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += seed;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}

	unsigned int Random::Hash(unsigned int v1, unsigned int v2, int unsigned seed)
	{
		unsigned int hash = 0;
		hash += v1;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += v2;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += seed;
		hash += hash << 10;
		hash ^= hash >> 6;

		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}

}