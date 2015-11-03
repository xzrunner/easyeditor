#ifndef _DRAG2D_SPRITE_SELECTED_H_
#define _DRAG2D_SPRITE_SELECTED_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class SpriteSelectedSJ : public Subject
{
public:
	struct Params {
		ISprite* spr;
		bool clear;
	};

public:
	void OnSelected(const Params& p, Observer* except = NULL);

	static SpriteSelectedSJ* Instance();

private:
	SpriteSelectedSJ(int id);

private:
	static SpriteSelectedSJ* m_instance;

}; // SpriteSelectedSJ

}

#endif // _DRAG2D_SPRITE_SELECTED_H_