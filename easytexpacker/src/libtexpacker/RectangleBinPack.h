#ifndef _LIBTEXPACKER_RECTANGLE_BIN_PACK_H_
#define _LIBTEXPACKER_RECTANGLE_BIN_PACK_H_

#include "Rect.h"

#include <vector>

namespace libtexpacker
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

#endif // _LIBTEXPACKER_RECTANGLE_BIN_PACK_H_