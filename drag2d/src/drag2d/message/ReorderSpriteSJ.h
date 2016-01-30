#ifndef _DRAG2D_REORDER_SPRITE_SJ_H_
#define _DRAG2D_REORDER_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class ReorderSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool up;
	};

public:
	void Reorder(Sprite* spr, bool up, Observer* except = NULL);

	static ReorderSpriteSJ* Instance();

private:
	ReorderSpriteSJ(int id);

private:
	static ReorderSpriteSJ* m_instance;

}; // ReorderSpriteSJ

}

#endif // _DRAG2D_REORDER_SPRITE_SJ_H_