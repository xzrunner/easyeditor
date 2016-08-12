#include "Scale9Symbol.h"

#include <assert.h>

namespace s2
{

Scale9Symbol::Scale9Symbol()
{
}

void Scale9Symbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	assert(spr);
}

sm::rect Scale9Symbol::GetBounding(const Sprite* spr) const
{
	return sm::rect(0, 0);
}

}