#ifndef _DRAG2D_SELECTE_SPRITE_SJ_H_
#define _DRAG2D_SELECTE_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class SelectSpriteSJ : public Subject
{
public:
	struct Params {
		Sprite* spr;
		bool clear;
	};

public:
	void Select(Sprite* spr, bool clear, Observer* except = NULL);

	static SelectSpriteSJ* Instance();

private:
	SelectSpriteSJ(int id);

private:
	static SelectSpriteSJ* m_instance;

}; // SelectSpriteSJ

}

#endif // _DRAG2D_SELECTE_SPRITE_SJ_H_