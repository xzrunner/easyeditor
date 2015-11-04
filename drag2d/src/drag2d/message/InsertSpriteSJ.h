#ifndef _DRAG2D_INSERT_SPRITE_SJ_H_
#define _DRAG2D_INSERT_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class InsertSpriteSJ : public Subject
{
public:
	struct Params {
		ISprite* spr;
		int idx;
	};

public:
	void Insert(const Params& p, Observer* except = NULL);

	static InsertSpriteSJ* Instance();

private:
	InsertSpriteSJ(int id);

private:
	static InsertSpriteSJ* m_instance;

}; // InsertSpriteSJ

}

#endif // _DRAG2D_INSERT_SPRITE_SJ_H_