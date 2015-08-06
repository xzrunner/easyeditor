#pragma once

#include <drag2d.h>

namespace eanim
{

class SelectSpritesOP : public d2d::SelectSpritesOP
{
public:
	SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
		d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback = NULL);

	virtual bool OnMouseLeftDClick(int x, int y);

protected:
	virtual void PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const;
	virtual void CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const;

}; // SelectSpritesOP

}