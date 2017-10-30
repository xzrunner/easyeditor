#ifndef _EASYEDITOR_SELECT_SPRITES_OP_H_
#define _EASYEDITOR_SELECT_SPRITES_OP_H_

#include "DrawSelectRectOP.h"
#include "Sprite.h"

#include <json/json.h>

#include <vector>

namespace ee
{

class MultiSpritesImpl;
class EditCMPT;
class IPropertySetting;
class SpriteSelection;

class SelectSpritesOP : public DrawSelectRectOP
{
public:
	SelectSpritesOP(wxWindow* wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, EditCMPT* callback = NULL);
	virtual ~SelectSpritesOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnKeyUp(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	virtual SprPtr SelectByPos(const sm::vec2& pos) const;

	virtual void PasteSprToClipboard(const SprConstPtr& spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(const SprPtr& spr, const Json::Value& value) const;

	void GetOrderedSelection(std::vector<SprPtr>& sprs) const;

private:
	void PasteToSelection() const;
	void CopyFromSelection();

protected:
	SpriteSelection* m_selection;

	MultiSpritesImpl* m_sprs_impl;

	EditCMPT* m_callback;

private:
	sm::vec2 m_left_first_pos, m_right_first_pos;

	// To disable mouse able when press ctrl and window query
	bool m_draggable;

	bool m_rect_select;

}; // SelectSpritesOP

}

#endif // _EASYEDITOR_SELECT_SPRITES_OP_H_