#include "DummySprite.h"
#include "DummySymbol.h"

namespace ee
{

DummySprite::DummySprite(const DummySprite& spr)
	: s2::Sprite(spr)
	, s2::DummySprite(spr)
	, Sprite(spr)
{
}

DummySprite::DummySprite(DummySymbol* sym)
	: s2::Sprite(sym)
	, s2::DummySprite(sym)
	, Sprite(sym)
{
}

}