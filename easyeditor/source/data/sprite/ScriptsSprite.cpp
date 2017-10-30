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

ScriptsSprite::ScriptsSprite(const s2::SymPtr& sym, uint32_t id)
	: s2::Sprite(sym)
	, Sprite(sym)
{
	scripts_do_string(S2_VI_PTR_DOWN_CAST<ScriptsSymbol>(sym)->GetContent().c_str());
}

}