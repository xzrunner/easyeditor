#include "SelectSpritesOP.h"

#include "dataset/SpriteUserData.h"
#include "dataset/LayersMgr.h"
#include "dataset/KeyFrame.h"
#include "dataset/DataMgr.h"

#include <ee/EditPanelImpl.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/Sprite.h>

#include <easycomplex.h>

namespace eanim
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(wnd, stage, sprites_impl, callback)
	, m_open_symbol(wnd, stage, sprites_impl)
{
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) {
		return true;
	}

	// group
	if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'G') {
		ecomplex::GroupHelper::BuildGroup(m_selection);
		return true;
	} else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'B') {
		ecomplex::GroupHelper::BreakUpGroup(m_selection);
		return true;
	}
	// complex
	else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'G') {
		ecomplex::GroupHelper::BuildComplex(m_selection, "", m_wnd);
		return true;
	} else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'B') {
		ecomplex::GroupHelper::BreakUpComplex(m_selection);
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) 
		return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_sprs_impl->QuerySpriteByPos(pos);
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
	SpriteUserData::SetSprData(spr, layer_idx, frame_idx);
}

}