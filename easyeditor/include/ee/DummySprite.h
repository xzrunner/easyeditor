#ifndef _EASYEDITOR_DUMMY_SPRITE_H_
#define _EASYEDITOR_DUMMY_SPRITE_H_

#include "Sprite.h"

namespace ee
{

class DummySymbol;

class DummySprite : public Sprite
{
public:
	DummySprite(const DummySprite& spr);
	DummySprite& operator = (const DummySprite& spr);
	DummySprite(const s2::SymPtr& sym, uint32_t id = -1);

	SPRITE_CLONE_FUNC(DummySprite)

}; // DummySprite

}

#endif // _EASYEDITOR_DUMMY_SPRITE_H_