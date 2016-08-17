#ifndef _EASYEDITOR_SPRITE_OBSERVER_H_
#define _EASYEDITOR_SPRITE_OBSERVER_H_

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class SpriteObserver
{
public:
	virtual void Translate(Sprite* spr, const sm::vec2& offset) = 0;
	virtual void Rotate(Sprite* spr, float delta) = 0;
	virtual ~SpriteObserver() {}
}; // SpriteObserver

}

#endif // _EASYEDITOR_SPRITE_OBSERVER_H_
