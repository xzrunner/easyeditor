#ifndef _DRAG2D_ISPRITE_VIEW_PANEL_H_
#define _DRAG2D_ISPRITE_VIEW_PANEL_H_

namespace d2d
{

class ISprite;
class IShape;
class SpriteSelection;

class ISpriteViewPanel
{
public:
	virtual ~ISpriteViewPanel() {}
	virtual bool ReorderSprite(ISprite* spr, bool up) = 0;
	virtual bool InsertSprite(ISprite* spr, int idx = -1) = 0;
	virtual bool RemoveSprite(ISprite* spr) = 0;
	virtual bool ClearAllSprite() = 0;
}; // ISpriteViewPanel

}

#endif // _DRAG2D_ISPRITE_VIEW_PANEL_H_