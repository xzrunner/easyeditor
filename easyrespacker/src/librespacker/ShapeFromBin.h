#ifndef _LIBRESPACKER_SHAPE_FROM_BIN_H_
#define _LIBRESPACKER_SHAPE_FROM_BIN_H_

#include "PackShape.h"

namespace librespacker
{

class ShapeFromBin
{
public:
	static int Size(const PackShape* shape);

	static void Unpack(uint8_t** ptr, PackShape* shape);

}; // ShapeFromBin

}

#endif // _LIBRESPACKER_SHAPE_FROM_BIN_H_