#ifndef _DRAG2D_REORDER_SPRITE_SJ_H_
#define _DRAG2D_REORDER_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class ReorderSpriteSJ : public Subject
{
public:
	struct Params {
		ISprite* spr;
		bool up;
	};

public:
	void Reorder(const Params& p, Observer* except = NULL);

	static ReorderSpriteSJ* Instance();

private:
	ReorderSpriteSJ(int id);

private:
	static ReorderSpriteSJ* m_instance;

}; // ReorderSpriteSJ

}

#endif // _DRAG2D_REORDER_SPRITE_SJ_H_