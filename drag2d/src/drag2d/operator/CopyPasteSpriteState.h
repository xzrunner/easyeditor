#ifndef _DRAG2D_COPY_PASTE_SPRITE_STATE_H_
#define _DRAG2D_COPY_PASTE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"

namespace d2d
{

class ISprite;
class SpriteSelection;

class CopyPasteSpriteState : public IArrangeSpriteState
{
public:
	CopyPasteSpriteState(SpriteSelection* selection, ISprite* selected);
	virtual ~CopyPasteSpriteState();

	virtual bool OnMouseDrag(const Vector& pos);

private:
	SpriteSelection* m_selection;

	ISprite* m_spr;

}; // CopyPasteSpriteState

}

#endif // _DRAG2D_COPY_PASTE_SPRITE_STATE_H_