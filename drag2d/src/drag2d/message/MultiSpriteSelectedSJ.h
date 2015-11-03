#ifndef _DRAG2D_MULTI_SPRITE_SELECTED_H_
#define _DRAG2D_MULTI_SPRITE_SELECTED_H_

#include "Subject.h"

namespace d2d
{

class SpriteSelection;

class MultiSpriteSelectedSJ : public Subject
{
public:
	void OnSelected(const SpriteSelection* selection, Observer* except = NULL);

	static MultiSpriteSelectedSJ* Instance();

private:
	MultiSpriteSelectedSJ(int id);

private:
	static MultiSpriteSelectedSJ* m_instance;

}; // MultiSpriteSelectedSJ

}

#endif // _DRAG2D_MULTI_SPRITE_SELECTED_H_