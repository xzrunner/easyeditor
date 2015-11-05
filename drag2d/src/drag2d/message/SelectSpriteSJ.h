#ifndef _DRAG2D_SELECTE_SPRITE_SJ_H_
#define _DRAG2D_SELECTE_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class SelectSpriteSJ : public Subject
{
public:
	struct Params {
		ISprite* spr;
		bool clear;
	};

public:
	void Select(ISprite* spr, bool clear, Observer* except = NULL);

	static SelectSpriteSJ* Instance();

private:
	SelectSpriteSJ(int id);

private:
	static SelectSpriteSJ* m_instance;

}; // SelectSpriteSJ

}

#endif // _DRAG2D_SELECTE_SPRITE_SJ_H_