#ifndef _DRAG2D_I_VIEW_PANEL_H_
#define _DRAG2D_I_VIEW_PANEL_H_

namespace d2d
{

class ISprite;

class IViewPanel
{
public:
	virtual ~IViewPanel() {}
	virtual void SelectSprite(ISprite* spr) = 0;
	virtual void ReorderSprite(ISprite* spr, bool up) = 0;
	virtual void InsertSprite(ISprite* spr) = 0;
	virtual void RemoveSprite(ISprite* spr) = 0;

}; // IViewPanel

}

#endif // _DRAG2D_I_VIEW_PANEL_H_