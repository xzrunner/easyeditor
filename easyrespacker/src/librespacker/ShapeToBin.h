#ifndef _LIBRESPACKER_SHAPE_TO_BIN_H_
#define _LIBRESPACKER_SHAPE_TO_BIN_H_

#include "PackShape.h"

namespace librespacker
{

class ShapeToBin
{
public:
	static int Size(const PackShape* shape);

	static void Pack(const PackShape* shape, uint8_t** ptr);

}; // ShapeToBin

}

#endif // _LIBRESPACKER_SHAPE_TO_BIN_H_