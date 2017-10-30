#include "DummySprite.h"
#include "DummySymbol.h"

namespace ee
{

DummySprite::DummySprite(const DummySprite& spr)
	: s2::Sprite(spr)
	, Sprite(spr)
{
}

DummySprite& DummySprite::operator = (const DummySprite& spr)
{
	s2::Sprite::operator = (spr);
	Sprite::operator = (spr);
	return *this;
}

DummySprite::DummySprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, Sprite(sym)
{
}

}