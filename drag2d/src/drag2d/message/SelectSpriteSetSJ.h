#ifndef _DRAG2D_SELECT_SPRITE_SET_SJ_H_
#define _DRAG2D_SELECT_SPRITE_SET_SJ_H_

#include "Subject.h"

namespace d2d
{

class SpriteSelection;

class SelectSpriteSetSJ : public Subject
{
public:
	void Select(const SpriteSelection* selection, Observer* except = NULL);

	static SelectSpriteSetSJ* Instance();

private:
	SelectSpriteSetSJ(int id);

private:
	static SelectSpriteSetSJ* m_instance;

}; // SelectSpriteSetSJ

}

#endif // _DRAG2D_SELECT_SPRITE_SET_SJ_H_