#include "ScriptsSprite.h"
#include "ScriptsSymbol.h"
#include "scripts.h"

#include <sprite2/DummySprite.h>

namespace ee
{

ScriptsSprite::ScriptsSprite(const ScriptsSprite& spr)
	: s2::Sprite(spr)
	, s2::DummySprite(spr)
	, Sprite(spr)
{
}

ScriptsSprite::ScriptsSprite(ScriptsSymbol* sym)
	: s2::Sprite(sym)
	, s2::DummySprite(sym)
	, Sprite(sym)
{
	scripts_do_string(sym->GetContent().c_str());
}

}