#ifndef EPACKER_DIRECTLY_ARRANGE_H
#define EPACKER_DIRECTLY_ARRANGE_H

#include "BaseStrategy.h"

namespace epacker
{
	class DirectlyArrange : public BaseStrategy
	{
	public:
		virtual void arrange(const std::vector<d2d::ImageSprite*>& sprites);

	}; // DirectlyArrange
}

#endif // EPACKER_DIRECTLY_ARRANGE_H