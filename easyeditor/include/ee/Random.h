#ifndef _EASYEDITOR_RANDOM_H_
#define _EASYEDITOR_RANDOM_H_

#include <vector>

namespace ee
{

class Random
{
public:
	template<class T>
		static void RandomPermutation(std::vector<T>& src);

	static float GetNum(float min, float max);
	static float GetNum0To1();
	static float GetRadian();

}; // Random

}

#endif // _EASYEDITOR_RANDOM_H_