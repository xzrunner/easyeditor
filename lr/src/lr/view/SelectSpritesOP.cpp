#include "SelectSpritesOP.h"
#include "StagePanel.h"

#include "view/typedef.h"

#include <ee/EditPanelImpl.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/sprite_msg.h>
#include <ee/Symbol.h>
#include <ee/panel_msg.h>
#include <ee/DeleteSpriteAOP.h>
#include <ee/InsertSpriteAOP.h>
#include <ee/CombineAOP.h>
#include <ee/SpriteFactory.h>
#include <ee/PointQueryVisitor.h>
#include <ee/TranslateSpriteAOP.h>
#include <ee/StringHelper.h>
#include <ee/SymbolFile.h>
#include <ee/FilepathDialog.h>
#include <ee/SymbolType.h>
#include <ee/FileHelper.h>

#include <sprite2/SymType.h>

#include <easycomplex.h>

namespace lr
{

SelectSpritesOP::SelectSpritesOP(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, sprites_impl, callback)
	, m_open_symbol(stage_wnd, stage, sprites_impl)
{
	m_first_press.MakeInvalid();
	stage->SetCursor(wxCursor(wxCURSOR_PENCIL));
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
		const std::string& dir = static_cast<StagePanel*>(m_wnd)->GetResDir();
		ecomplex::GroupHelper::BuildComplex(m_selection, dir, m_wnd);
		return true;
	} else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'B') {
		ecomplex::GroupHelper::BreakUpComplex(m_selection);
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDown(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	StagePanel* stage = static_cast<StagePanel*>(m_wnd);
	stage->PointQuery(pos);

	if (m_first_press.IsValid()) {
		stage->Pathfinding(m_first_press, pos);
		m_first_press.MakeInvalid();
	} else {
		m_first_press = pos;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);

	ee::Sprite* selected = NULL;
	m_selection->Traverse(ee::PointQueryVisitor(pos, &selected));
	if (!selected) {
		selected = m_sprs_impl->QuerySpriteByPos(pos);
	}
	if (selected) {
		m_open_symbol.Open(selected);
	}

	return false;
}

}