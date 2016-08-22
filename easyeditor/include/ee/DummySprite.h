#ifndef _EASYEDITOR_DUMMY_SPRITE_H_
#define _EASYEDITOR_DUMMY_SPRITE_H_

#include "Sprite.h"

#include <sprite2/DummySprite.h>

namespace ee
{

class DummySymbol;

class DummySprite : public s2::DummySprite, public Sprite
{
public:
	DummySprite(const DummySprite& spr);
	DummySprite(DummySymbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual DummySprite* Clone() const { return new DummySprite(*this); }

}; // DummySprite

}

#endif // _EASYEDITOR_DUMMY_SPRITE_H_