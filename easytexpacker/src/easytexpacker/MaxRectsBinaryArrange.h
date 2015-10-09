#ifndef _MAX_RECTS_BINARY_ARRANGE_H_
#define _MAX_RECTS_BINARY_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{

class MaxRectsBinaryArrange : public BaseStrategy
{
public:
	virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);
	virtual int GetTextureAccount() const { return 1; }

}; // MaxRectsBianryArrange

}

#endif // _MAX_RECTS_BINARY_ARRANGE_H_