#include "MaskSprite.h"

namespace s2
{

MaskSprite::MaskSprite() 
{
}

MaskSprite::MaskSprite(Symbol* sym) 
	: Sprite(sym) 
{
}

MaskSprite* MaskSprite::Clone() const
{
	return new MaskSprite(*this);
}

bool MaskSprite::Update(const RenderParams& params)
{
	// todo
	return false;
}

}