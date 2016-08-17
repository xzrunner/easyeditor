#ifndef _MAX_RECTS_BINARY_ARRANGE_H_
#define _MAX_RECTS_BINARY_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{

class MaxRectsBinaryArrange : public BaseStrategy
{
public:
	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs);
	virtual int GetTextureAccount() const { return 1; }

}; // MaxRectsBianryArrange

}

#endif // _MAX_RECTS_BINARY_ARRANGE_H_