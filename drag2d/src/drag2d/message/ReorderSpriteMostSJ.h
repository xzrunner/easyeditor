#ifndef _DRAG2D_REORDER_SPRITE_MOST_SJ_H_
#define _DRAG2D_REORDER_SPRITE_MOST_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class ReorderSpriteMostSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool up;
	};

public:
	void Reorder(Sprite* spr, bool up, Observer* except = NULL);

	static ReorderSpriteMostSJ* Instance();

private:
	ReorderSpriteMostSJ(int id);

private:
	static ReorderSpriteMostSJ* m_instance;

}; // ReorderSpriteMostSJ

}

#endif // _DRAG2D_REORDER_SPRITE_MOST_SJ_H_