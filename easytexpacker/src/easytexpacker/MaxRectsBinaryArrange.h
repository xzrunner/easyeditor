#ifndef _MAX_RECTS_BINARY_ARRANGE_H_
#define _MAX_RECTS_BINARY_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{

class MaxRectsBinaryArrange : public BaseStrategy
{
public:
	virtual void Arrange(const std::vector<ee::ImageSprite*>& sprs) override;
	virtual int GetTextureAccount() const override { return 1; }

}; // MaxRectsBianryArrange

}

#endif // _MAX_RECTS_BINARY_ARRANGE_H_