#ifndef _LIBCOCO_COCO_UNPACKER_H_
#define _LIBCOCO_COCO_UNPACKER_H_

#include <drag2d.h>

namespace libcoco
{

class CocoUnpacker
{
public:
	CocoUnpacker(const std::string& filepath, 
		const std::vector<d2d::Image*>& images);

}; // CocoUnpacker

}

#endif // _LIBCOCO_COCO_UNPACKER_H_