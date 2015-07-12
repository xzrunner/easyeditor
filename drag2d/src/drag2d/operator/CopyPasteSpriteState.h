#ifndef _DRAG2D_COPY_PASTE_SPRITE_STATE_H_
#define _DRAG2D_COPY_PASTE_SPRITE_STATE_H_

#include "IArrangeSpriteState.h"

namespace d2d
{

class ISprite;
class MultiSpritesImpl;
class SpriteSelection;

class CopyPasteSpriteState : public IArrangeSpriteState
{
public:
	CopyPasteSpriteState(MultiSpritesImpl* sprites_impl,
		SpriteSelection* selection, ISprite* selected);
	virtual ~CopyPasteSpriteState();

	virtual bool OnMouseDrag(const Vector& pos);

private:
	MultiSpritesImpl* m_sprites_impl;

	SpriteSelection* m_selection;

	ISprite* m_spr;

}; // CopyPasteSpriteState

}

#endif // _DRAG2D_COPY_PASTE_SPRITE_STATE_H_