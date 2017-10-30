#ifndef _EASYEDITOR_RANDOM_H_
#define _EASYEDITOR_RANDOM_H_

#include <cu/cu_stl.h>

namespace ee
{

class Random
{
public:
	template<class T>
		static void RandomPermutation(CU_VEC<T>& src);

	static float GetNum(float min, float max);
	static float GetNum0To1();
	static float GetRadian();

}; // Random

}

#include "Random.inl"

#endif // _EASYEDITOR_RANDOM_H_