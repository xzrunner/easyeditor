#ifndef _LIBPACKER_RECTANGLE_BIN_PACK_H_
#define _LIBPACKER_RECTANGLE_BIN_PACK_H_

#include "Rect.h"

#include <vector>

namespace libpacker
{

class RectangleBinPack
{
public:
	RectangleBinPack();
	~RectangleBinPack();

	RectSize GetSize() const;

	void Pack(const std::vector<RectSize>& rects, std::vector<Rect>& output);

}; // RectangleBinPack

}

#endif // _LIBPACKER_RECTANGLE_BIN_PACK_H_