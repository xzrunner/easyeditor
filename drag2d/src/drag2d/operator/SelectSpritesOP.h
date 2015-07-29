#ifndef _DRAG2D_SELECT_SPRITES_OP_H_
#define _DRAG2D_SELECT_SPRITES_OP_H_

#include <vector>

#include "DrawSelectRectOP.h"

#include "render/ShapeStyle.h"

namespace d2d
{

class MultiSpritesImpl;
class AbstractEditCMPT;
class IPropertySetting;
class ViewPanelMgr;
class SpriteSelection;

class SelectSpritesOP : public DrawSelectRectOP
{
public:
	SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
		ViewPanelMgr* view_panel_mgr = NULL, AbstractEditCMPT* callback = NULL);
	virtual ~SelectSpritesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	virtual ISprite* SelectByPos(const Vector& pos) const;

	virtual void PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const;

private:
	void PasteToSelection() const;
	void CopyFromSelection();

protected:
	SpriteSelection* m_selection;

	MultiSpritesImpl* m_spritesImpl;

	AbstractEditCMPT* m_callback;

private:
	Vector m_first_pos;
	Vector m_rightFirstScrPos;

	// To disable mouse able when press ctrl and window query
	bool m_bDraggable;

private:
	ViewPanelMgr* m_view_panel_mgr;

}; // SelectSpritesOP

}

#endif // _DRAG2D_SELECT_SPRITES_OP_H_