#include "ScriptsSprite.h"
#include "ScriptsSymbol.h"
#include "scripts.h"

#include <sprite2/DummySprite.h>

namespace ee
{

ScriptsSprite::ScriptsSprite(ScriptsSymbol* sym)
	: Sprite(sym)
{
	scripts_do_string(sym->GetContent().c_str());
}

}