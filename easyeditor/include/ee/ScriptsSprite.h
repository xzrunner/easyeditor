#ifndef _EASYEDITOR_SCRIPTS_SPRITE_H_
#define _EASYEDITOR_SCRIPTS_SPRITE_H_

#include "Sprite.h"

namespace ee
{

class ScriptsSymbol;

class ScriptsSprite : public Sprite
{
public:
	ScriptsSprite& operator = (const ScriptsSprite& spr);
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