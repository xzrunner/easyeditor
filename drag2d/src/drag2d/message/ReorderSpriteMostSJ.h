#ifndef _DRAG2D_REORDER_SPRITE_MOST_SJ_H_
#define _DRAG2D_REORDER_SPRITE_MOST_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class ReorderSpriteMostSJ : public Subject
{
public:
	struct Params {
		ISprite* spr;
		bool up;
	};

public:
	void Reorder(ISprite* spr, bool up, Observer* except = NULL);

	static ReorderSpriteMostSJ* Instance();

private:
	ReorderSpriteMostSJ(int id);

private:
	static ReorderSpriteMostSJ* m_instance;

}; // ReorderSpriteMostSJ

}

#endif // _DRAG2D_REORDER_SPRITE_MOST_SJ_H_