#ifndef _DRAG2D_SPRITE_NAME_CHANGE_SJ_H_
#define _DRAG2D_SPRITE_NAME_CHANGE_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class SpriteNameChangeSJ : public Subject
{
public:
	void OnSpriteNameChanged(const Sprite* spr, Observer* except = NULL);

	static SpriteNameChangeSJ* Instance();

private:
	SpriteNameChangeSJ(int id);

private:
	static SpriteNameChangeSJ* m_instance;

}; // SpriteNameChangeSJ

}

#endif // _DRAG2D_SPRITE_NAME_CHANGE_SJ_H_