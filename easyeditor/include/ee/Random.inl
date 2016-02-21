#ifndef _EASYEDITOR_RANDOM_INL_
#define _EASYEDITOR_RANDOM_INL_

#include <time.h>

namespace ee
{

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

inline 
float Random::GetNum(float min, float max)
{
	return (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
}

inline 
float Random::GetNum0To1()
{
	return rand() / static_cast<float>(RAND_MAX);
}

inline 
float Random::GetRadian()
{
	return 3.14f * 2 * GetNum0To1();
}

}

#endif // _EASYEDITOR_RANDOM_INL_
