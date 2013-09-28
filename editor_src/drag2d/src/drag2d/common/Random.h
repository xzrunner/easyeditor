#pragma once

#include <time.h>

namespace d2d
{
	class Random
	{
	public:
		template<class T>
			static void RandomPermutation(std::vector<T>& src);

		static float getNum(float min, float max);
		static float getNum0To1();
		static float getRadian();

	}; // Random

	template<class T>
	void Random::RandomPermutation(std::vector<T>& src)
	{
		srand((unsigned)time(NULL));
		size_t range = src.size() - 1;

		for(size_t i = src.size() - 1; i > 1; --i) {
			size_t rndIndex = int(__int64(range) * rand() / (RAND_MAX + 1.0 ));
			std::swap(src.at(i), src.at(rndIndex));
		}
	}

	inline float Random::getNum(float min, float max)
	{
		return (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	}

	inline float Random::getNum0To1()
	{
		return rand() / static_cast<float>(RAND_MAX);
	}

	inline float Random::getRadian()
	{
		return 3.14f * 2 * getNum0To1();
	}
}

