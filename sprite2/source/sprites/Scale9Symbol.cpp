#include "Scale9Symbol.h"

#include <assert.h>

namespace s2
{

Scale9Symbol::Scale9Symbol(void* ud)
	: Symbol(ud)
{
}

void Scale9Symbol::Draw(const RenderParams& params, const Sprite* spr) const
{
	assert(spr);
	
}

}