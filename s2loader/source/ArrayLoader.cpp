#include "s2loader/ArrayLoader.h"

#include <bs/FixedPointNum.h>

#include <string.h>

namespace s2loader
{

void ArrayLoader::Load(CU_VEC<sm::vec2>& dst, const uint16_t* src, int src_n, int precision)
{
	dst.reserve(src_n);
	int idx = 0;
	for (int i = 0; i < src_n; ++i) {
		float x = bs::int2float(int16_t(src[idx++]), precision),
			  y = bs::int2float(int16_t(src[idx++]), precision);
		dst.push_back(sm::vec2(x, y));
	}	
}

void ArrayLoader::Load(CU_VEC<int>& dst, const uint16_t* src, int src_n)
{
	dst.reserve(src_n);
	for (int i = 0; i < src_n; ++i) {
		dst.push_back(src[i]);
	}
}

void ArrayLoader::Load(CU_VEC<sm::vec2>& dst, const uint32_t* src, int src_n)
{
	dst.reserve(src_n);
	int idx = 0;
	for (int i = 0; i < src_n; ++i) {
		float x, y;
		memcpy(&x, &src[idx++], sizeof(float));
		memcpy(&y, &src[idx++], sizeof(float));
		dst.push_back(sm::vec2(x, y));
	}	
}

}