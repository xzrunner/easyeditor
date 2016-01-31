#ifndef _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_
#define _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_

#include "ArrangeSpriteState.h"

namespace ee
{

class Sprite;
class SpriteSelection;

class CopyPasteSpriteState : public ArrangeSpriteState
{
public:
	CopyPasteSpriteState(SpriteSelection* selection, Sprite* selected);
	virtual ~CopyPasteSpriteState();

	virtual bool OnMouseDrag(const Vector& pos);

private:
	SpriteSelection* m_selection;

	Sprite* m_spr;

}; // CopyPasteSpriteState

}

#endif // _EASYEDITOR_COPY_PASTE_SPRITE_STATE_H_