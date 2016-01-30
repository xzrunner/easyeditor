#ifndef _DRAG2D_INSERT_SPRITE_SJ_H_
#define _DRAG2D_INSERT_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class InsertSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		int idx;
	};

public:
	void Insert(Sprite* spr, int idx = -1, Observer* except = NULL);

	static InsertSpriteSJ* Instance();

private:
	InsertSpriteSJ(int id);

private:
	static InsertSpriteSJ* m_instance;

}; // InsertSpriteSJ

}

#endif // _DRAG2D_INSERT_SPRITE_SJ_H_