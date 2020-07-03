#pragma once

namespace Ostaga {

	class Random
	{
	public:
		inline static void SetSeed(unsigned int seed) { s_Seed = seed; }
		static unsigned int Integer();
		static float Float();

	private:
		static unsigned int Hash(unsigned int value);

	private:
		union FloatBitsToInt
		{
			unsigned int IntBits;
			float FloatBits;
		};

		static unsigned int s_Seed;

	};
}