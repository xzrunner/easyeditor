#include "SelectSpritesOP.h"

#include "dataset/SpriteUserData.h"
#include "dataset/LayersMgr.h"
#include "dataset/KeyFrame.h"

#include <easymesh.h>
#include <easyicon.h>

namespace eanim
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiSpritesImpl* spritesImpl, 
								 d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::SelectSpritesOP(wnd, stage, spritesImpl, callback)
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
	}

	return false;
}

void SelectSpritesOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	d2d::SelectSpritesOP::PasteSprToClipboard(spr, value);

	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	value["anim"]["layer"] = ud->all_layers->QueryIndex(ud->layer);
	value["anim"]["frame"] = ud->frame->GetTime() - 1;
}

void SelectSpritesOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	d2d::SelectSpritesOP::CopySprFromClipboard(spr, value);

	int layer_idx = value["anim"]["layer"].asInt(),
		frame_idx = value["anim"]["frame"].asInt();
	set_sprite_user_data(spr, layer_idx, frame_idx);
}

}