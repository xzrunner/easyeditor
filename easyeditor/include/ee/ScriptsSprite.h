#ifndef _EASYEDITOR_SCRIPTS_SPRITE_H_
#define _EASYEDITOR_SCRIPTS_SPRITE_H_

#include "Sprite.h"

#include <sprite2/DummySprite.h>

namespace ee
{

class ScriptsSymbol;

class ScriptsSprite : public s2::DummySprite, public Sprite
{
public:
	ScriptsSprite(const ScriptsSprite& spr);
	ScriptsSprite(ScriptsSymbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual ScriptsSprite* Clone() const { return new ScriptsSprite(*this); }

}; // ScriptsSprite 

}

#endif // _EASYEDITOR_SCRIPTS_SPRITE_H_