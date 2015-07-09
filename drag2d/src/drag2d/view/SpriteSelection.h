#ifndef _DRAG2D_SPRITE_SELECTION_H_
#define _DRAG2D_SPRITE_SELECTION_H_

#include "dataset/ISprite.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class IStageCanvas;

class SpriteSelection : public ObjSelectionSet<ISprite>
{
public:
	SpriteSelection(EditPanel* stage);

	virtual void Clear();

	virtual void Add(ISprite* item);
	virtual void Remove(ISprite* item);

private:
	EditPanel* m_stage;

}; // SpriteSelection

}

#endif // _DRAG2D_SPRITE_SELECTION_H_