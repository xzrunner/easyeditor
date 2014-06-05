#ifndef EPACKER_RECT_BIN_ARRANGE_H
#define EPACKER_RECT_BIN_ARRANGE_H

#include "BaseStrategy.h"

namespace epacker
{
	class RectBinArrange : public BaseStrategy
	{
	public:
		virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);
		virtual int GetTextureAccount() const { return 1; }

	}; // RectBinArrange
}

#endif // EPACKER_RECT_BIN_ARRANGE_H