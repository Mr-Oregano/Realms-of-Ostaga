#pragma once

namespace Ostaga {

	class Random
	{
	public:
		inline static void SetSeed(unsigned int seed) { s_Seed = seed; }
		static unsigned int Integer();

		static float Float();
		static float Float(float x);
		static float Float(float x, float y);

		static float Simplex(float x, float y);

	private:
		static unsigned int Hash(unsigned int seed);
		static unsigned int Hash(unsigned int v1, unsigned int seed);
		static unsigned int Hash(unsigned int v1, unsigned int v2, int unsigned seed);
		static unsigned int ConvertFloatBitsToInt(float f);

	private:
		union FloatBitsToInt
		{
			unsigned int IntBits;
			float FloatBits;
		};

		static unsigned int s_Seed;

	};
}