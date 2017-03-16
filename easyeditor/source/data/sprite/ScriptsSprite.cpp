#include "ScriptsSprite.h"
#include "ScriptsSymbol.h"
#include "scripts.h"

namespace ee
{

ScriptsSprite::ScriptsSprite(const ScriptsSprite& spr)
	: s2::Sprite(spr)
	, Sprite(spr)
{
}

ScriptsSprite& ScriptsSprite::operator = (const ScriptsSprite& spr)
{
	s2::Sprite::operator = (spr);
	Sprite::operator = (spr);
	return *this;
}

ScriptsSprite::ScriptsSprite(ScriptsSymbol* sym)
	: s2::Sprite(sym)
	, Sprite(sym)
{
	scripts_do_string(sym->GetContent().c_str());
}

}