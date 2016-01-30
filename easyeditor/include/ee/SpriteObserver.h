#ifndef _EASYEDITOR_SPRITE_OBSERVER_H_
#define _EASYEDITOR_SPRITE_OBSERVER_H_

namespace ee
{

class Sprite;
class Vector;

class SpriteObserver
{
public:
	virtual void Translate(Sprite* sprite, const Vector& offset) = 0;
	virtual void Rotate(Sprite* sprite, float delta) = 0;
	virtual ~SpriteObserver() {}
}; // SpriteObserver

}

#endif // _EASYEDITOR_SPRITE_OBSERVER_H_
