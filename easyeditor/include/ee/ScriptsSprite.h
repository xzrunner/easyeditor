#ifndef _EASYEDITOR_SCRIPTS_SPRITE_H_
#define _EASYEDITOR_SCRIPTS_SPRITE_H_

#include "Sprite.h"

#include <sprite2/macro.h>

namespace ee
{

class ScriptsSymbol;

class ScriptsSprite : public Sprite
{
public:
	ScriptsSprite& operator = (const ScriptsSprite& spr);
	ScriptsSprite(const ScriptsSprite& spr);
	ScriptsSprite(const s2::SymPtr& sym, uint32_t id = -1);

	SPRITE_CLONE_FUNC(ScriptsSprite)

}; // ScriptsSprite 

}

#endif // _EASYEDITOR_SCRIPTS_SPRITE_H_