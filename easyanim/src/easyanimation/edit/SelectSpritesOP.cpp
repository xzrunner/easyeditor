#include "SelectSpritesOP.h"

#include "dataset/SpriteUserData.h"
// #include "frame/Controller.h"
// #include "view/StagePanel.h"

#include <easymesh.h>
#include <easyicon.h>

namespace eanim
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::ViewPanelMgr* view_panel_mgr, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(wnd, stage, spritesImpl, view_panel_mgr, callback)
{
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (d2d::SelectSpritesOP::OnMouseLeftDClick(x, y)) 
		return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	d2d::ISprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (emesh::Sprite* sprite = dynamic_cast<emesh::Sprite*>(selected))
	{
		emesh::EditDialog dlg(m_wnd, sprite);
		dlg.ShowModal();

		m_stage->ResetCanvas();
	}

	return false;
}

void SelectSpritesOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	d2d::SelectSpritesOP::PasteSprToClipboard(spr, value);

	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	value["layer"] = ud->layer;
	value["frame"] = ud->frame;
}

void SelectSpritesOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	d2d::SelectSpritesOP::CopySprFromClipboard(spr, value);

	set_sprite_user_data(spr, value["layer"].asInt(), value["frame"].asInt());
}

}