#ifndef _SPRITE2_DUMMY_SPRITE_H_
#define _SPRITE2_DUMMY_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class DummySprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	DummySprite() {}
	DummySprite(const DummySprite& spr) : Sprite(spr) {}

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual DummySprite* Clone() const { return new DummySprite(*this); }

}; // DummySprite

}

#endif // _SPRITE2_DUMMY_SPRITE_H_