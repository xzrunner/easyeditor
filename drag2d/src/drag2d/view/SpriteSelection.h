#ifndef _DRAG2D_SPRITE_SELECTION_H_
#define _DRAG2D_SPRITE_SELECTION_H_

#include "dataset/Sprite.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class EditPanelImpl;

class SpriteSelection : public ObjSelectionSet<Sprite>
{
public:
	virtual void Clear();

	virtual void Add(Sprite* item);
	virtual void Remove(Sprite* item);

}; // SpriteSelection

}

#endif // _DRAG2D_SPRITE_SELECTION_H_