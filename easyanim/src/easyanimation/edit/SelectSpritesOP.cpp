#include "SelectSpritesOP.h"

#include "dataset/SpriteUserData.h"
#include "dataset/LayersMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/DataMgr.h"

#include <ee/EditPanelImpl.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/Sprite.h>

namespace eanim
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(wnd, stage, sprites_impl, callback)
	, m_open_symbol(wnd, stage, sprites_impl)
{
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) 
		return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected);
	}

	return false;
}

void SelectSpritesOP::PasteSprToClipboard(const ee::Sprite* spr, Json::Value& value) const
{
	ee::SelectSpritesOP::PasteSprToClipboard(spr, value);

	SpriteUserData* ud = (SpriteUserData*)spr->GetUserData();
	value["anim"]["layer"] = DataMgr::Instance()->GetLayers().QueryIndex(ud->layer);
	value["anim"]["frame"] = ud->frame->GetTime() - 1;
}

void SelectSpritesOP::CopySprFromClipboard(ee::Sprite* spr, const Json::Value& value) const
{
	ee::SelectSpritesOP::CopySprFromClipboard(spr, value);

	int layer_idx = value["anim"]["layer"].asInt(),
		frame_idx = value["anim"]["frame"].asInt();
	set_sprite_user_data(spr, layer_idx, frame_idx);
}

}