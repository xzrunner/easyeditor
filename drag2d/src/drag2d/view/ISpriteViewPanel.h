#ifndef _DRAG2D_ISPRITE_VIEW_PANEL_H_
#define _DRAG2D_ISPRITE_VIEW_PANEL_H_

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class ISprite;
class IShape;

class ISpriteViewPanel
{
public:
	virtual ~ISpriteViewPanel() {}
	virtual void SelectSprite(ISprite* spr, bool clear) = 0;
	virtual void SelectMultiSprites(SpriteSelection* selection) = 0;
	virtual bool ReorderSprite(ISprite* spr, bool up) = 0;
	virtual bool InsertSprite(ISprite* spr) = 0;
	virtual bool RemoveSprite(ISprite* spr) = 0;
	virtual bool ClearAllSprite() = 0;
}; // ISpriteViewPanel

}

#endif // _DRAG2D_ISPRITE_VIEW_PANEL_H_