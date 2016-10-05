#ifndef _EASYEDITOR_SPRITE_OBSERVER_H_
#define _EASYEDITOR_SPRITE_OBSERVER_H_

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class SpriteObserver
{
public:
	virtual void OnSetPosition(Sprite* spr, const sm::vec2& pos) = 0;
	virtual void OnSetAngle(Sprite* spr, float angle) = 0;
	virtual void OnSetScale(Sprite* spr, const sm::vec2& scale) = 0;
	virtual void OnSetShear(Sprite* spr, const sm::vec2& shear) = 0;
	virtual void OnSetOffset(Sprite* spr, const sm::vec2& offset) = 0;
	virtual ~SpriteObserver() {}
}; // SpriteObserver

}

#endif // _EASYEDITOR_SPRITE_OBSERVER_H_
