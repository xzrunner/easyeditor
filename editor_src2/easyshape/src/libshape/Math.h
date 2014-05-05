#ifndef _LIBSHAPE_MATH_H_
#define _LIBSHAPE_MATH_H_

#include <drag2d.h>

namespace libshape
{

class ChainShape;

class Math
{
public:
	static void mergeTwoChains(const ChainShape& src0, const ChainShape& src1,
		std::vector<d2d::Vector>& dst);

	static void mergeMultiChains(const std::vector<ChainShape*>& src, 
		std::vector<d2d::Vector>& dst);

private:
	// for mergeMultiChains
	static void findNearestPair(const std::vector<ChainShape*>& chains, int& index0, int& index1);
	static float getDistanceOfChains(const ChainShape& chain0, const ChainShape& chain1);

}; // Math

}

#endif // _LIBSHAPE_MATH_H_
