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
	virtual void SelectSprite(ISprite* spr) = 0;
	virtual void SelectMultiSprites(SpriteSelection* selection) = 0;
	virtual void ReorderSprite(ISprite* spr, bool up) = 0;
	virtual void InsertSprite(ISprite* spr) = 0;
	virtual void RemoveSprite(ISprite* spr) = 0;
}; // ISpriteViewPanel

}

#endif // _DRAG2D_ISPRITE_VIEW_PANEL_H_