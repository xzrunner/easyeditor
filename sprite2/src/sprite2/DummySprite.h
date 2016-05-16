#ifndef _SPRITE2_DUMMY_SPRITE_H_
#define _SPRITE2_DUMMY_SPRITE_H_

#include "Sprite.h"

namespace s2
{

class DummySprite : public Sprite
{
public:
	DummySprite() : Sprite(NULL) {}

	virtual bool Update(float dt) { return false; }

}; // DummySprite

}

#endif // _SPRITE2_DUMMY_SPRITE_H_