#ifndef _DRAG2D_REMOVE_SPRITE_SJ_H_
#define _DRAG2D_REMOVE_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class RemoveSpriteSJ : public Subject
{
public:
	void Remove(ISprite* spr, Observer* except = NULL);

	static RemoveSpriteSJ* Instance();

private:
	RemoveSpriteSJ(int id);

private:
	static RemoveSpriteSJ* m_instance;

}; // RemoveSpriteSJ

}

#endif // _DRAG2D_REMOVE_SPRITE_SJ_H_