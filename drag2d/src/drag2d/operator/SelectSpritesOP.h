#ifndef _DRAG2D_SELECT_SPRITES_OP_H_
#define _DRAG2D_SELECT_SPRITES_OP_H_

#include "DrawSelectRectOP.h"

#include "render/ShapeStyle.h"

#include <vector>
#include <json/json.h>

namespace d2d
{

class MultiSpritesImpl;
class AbstractEditCMPT;
class IPropertySetting;
class SpriteSelection;
class ISprite;

class SelectSpritesOP : public DrawSelectRectOP
{
public:
	SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, AbstractEditCMPT* callback = NULL);
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

}; // SelectSpritesOP

}

#endif // _DRAG2D_SELECT_SPRITES_OP_H_