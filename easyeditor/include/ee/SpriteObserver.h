#ifndef _EASYEDITOR_SPRITE_OBSERVER_H_
#define _EASYEDITOR_SPRITE_OBSERVER_H_

#include <SM_Vector.h>

namespace ee
{

class Sprite;

class SpriteObserver
{
public:
	virtual void OnSetPosition(const Sprite& spr, const sm::vec2& pos) = 0;
	virtual void OnSetAngle(const Sprite& spr, float angle) = 0;
	virtual void OnSetScale(const Sprite& spr, const sm::vec2& scale) = 0;
	virtual void OnSetShear(const Sprite& spr, const sm::vec2& shear) = 0;
	virtual void OnSetOffset(const Sprite& spr, const sm::vec2& offset) = 0;
	virtual ~SpriteObserver() {}
}; // SpriteObserver

}

#endif // _EASYEDITOR_SPRITE_OBSERVER_H_
