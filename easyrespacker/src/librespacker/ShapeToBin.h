#ifndef _EASYRESPACKER_SHAPE_TO_BIN_H_
#define _EASYRESPACKER_SHAPE_TO_BIN_H_

#include "PackShape.h"

namespace erespacker
{

class ShapeToBin
{
public:
	static int Size(const PackShape* shape);

	static void Pack(const PackShape* shape, uint8_t** ptr);

}; // ShapeToBin

}

#endif // _EASYRESPACKER_SHAPE_TO_BIN_H_