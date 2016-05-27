#ifndef _SPRITE2_DUMMY_SPRITE_H_
#define _SPRITE2_DUMMY_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class DummySprite : public Sprite
{
public:
	DummySprite(void* ud) : Sprite(ud) {}

	virtual bool Update(float dt) { return false; }

private:
	DummySprite() {}
	const DummySprite& operator = (const DummySprite& spr) { return *this; }

}; // DummySprite

}

#endif // _SPRITE2_DUMMY_SPRITE_H_