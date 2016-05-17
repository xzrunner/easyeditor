#include "Scale9Sprite.h"

#include <stddef.h>

namespace s2
{

Scale9Sprite::Scale9Sprite(void* ud)
	: Sprite(ud)
	, m_sym(NULL)
{
}

void Scale9Sprite::Draw(const RenderParams& params) const
{
	
	
	for (int i = 0; i < 9; ++i) {
		Sprite* spr = m_grids[i];
		if (spr) {
		}
	}
}

}