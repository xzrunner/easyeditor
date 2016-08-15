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
	ScriptsSprite(ScriptsSymbol* sym);

	/**
	 *  @interface
	 *    ee::Sprite
	 */
	virtual ScriptsSprite* EEClone() const { return new ScriptsSprite(*this); }

}; // ScriptsSprite 

}

#endif // _EASYEDITOR_SCRIPTS_SPRITE_H_