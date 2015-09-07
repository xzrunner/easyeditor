#ifndef _LIBCOCO_COCO_UNPACKE_FROM_BIN_H_
#define _LIBCOCO_COCO_UNPACKE_FROM_BIN_H_

#include <drag2d.h>

namespace libcoco
{

class UnpackFromBin
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<d2d::Image*>& images);

private:
	static void Unpack(uint8_t** ptr, const std::vector<d2d::Image*>& images);

}; // UnpackFromBin

}

#endif // _LIBCOCO_COCO_UNPACKE_FROM_BIN_H_