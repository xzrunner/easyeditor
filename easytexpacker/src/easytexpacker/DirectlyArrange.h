#ifndef _EASYTEXPACKER_DIRECTLY_ARRANGE_H_
#define _EASYTEXPACKER_DIRECTLY_ARRANGE_H_

#include "BaseStrategy.h"

namespace etexpacker
{
	class DirectlyArrange : public BaseStrategy
	{
	public:
		virtual void arrange(const std::vector<ee::ImageSprite*>& sprites);
		virtual int GetTextureAccount() const { return 1; }

	}; // DirectlyArrange
}

#endif // _EASYTEXPACKER_DIRECTLY_ARRANGE_H_