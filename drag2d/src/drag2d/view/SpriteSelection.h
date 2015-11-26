#ifndef _DRAG2D_SPRITE_SELECTION_H_
#define _DRAG2D_SPRITE_SELECTION_H_

#include "dataset/ISprite.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class EditPanelImpl;

class SpriteSelection : public ObjSelectionSet<ISprite>
{
public:
	virtual void Clear();

	virtual void Add(ISprite* item);
	virtual void Remove(ISprite* item);

}; // SpriteSelection

}

#endif // _DRAG2D_SPRITE_SELECTION_H_