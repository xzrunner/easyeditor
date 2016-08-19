#include "Scale9Sprite.h"

#include <stddef.h>

namespace s2
{

Scale9Sprite::Scale9Sprite() 
{

}

Scale9Sprite::Scale9Sprite(Symbol* sym) 
	: Sprite(sym) 
{
}

Scale9Sprite* Scale9Sprite::Clone() const
{
	return new Scale9Sprite(*this);
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