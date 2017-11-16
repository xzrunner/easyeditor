#ifndef _EASYSHAPE_MATH_H_
#define _EASYSHAPE_MATH_H_

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>
#include <memory>

namespace eshape
{

class ChainShape;

class Math
{
public:
	static void mergeTwoChains(const ChainShape& src0, const ChainShape& src1,
		CU_VEC<sm::vec2>& dst);

	static void mergeMultiChains(const CU_VEC<std::shared_ptr<ChainShape>>& src, 
		CU_VEC<sm::vec2>& dst);

private:
	// for mergeMultiChains
	static void findNearestPair(const CU_VEC<std::shared_ptr<ChainShape>>& chains, int& index0, int& index1);
	static float getDistanceOfChains(const ChainShape& chain0, const ChainShape& chain1);

}; // Math

}

#endif // _EASYSHAPE_MATH_H_
