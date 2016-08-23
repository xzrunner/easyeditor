#include "IconSprite.h"

#include <stddef.h>

namespace s2
{

IconSprite::IconSprite() 
	: m_process(1)
{
}

IconSprite::IconSprite(const IconSprite& spr)
	: Sprite(spr)
	, m_process(spr.m_process)
{
}

IconSprite::IconSprite(Symbol* sym) 
	: Sprite(sym) 
	, m_process(1)
{
}

IconSprite* IconSprite::Clone() const
{
	return new IconSprite(*this);
}

}