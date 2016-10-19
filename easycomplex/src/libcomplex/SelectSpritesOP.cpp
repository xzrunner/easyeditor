#include "SelectSpritesOP.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Sprite.h"
#include "GroupHelper.h"

#include <ee/EditPanelImpl.h>
#include <ee/MultiSpritesImpl.h>
#include <ee/SpriteSelection.h>
#include <ee/FetchAllVisitor.h>
#include <ee/FileHelper.h>
#include <ee/StringHelper.h>
#include <ee/sprite_msg.h>
#include <ee/CrossGuides.h>
#include <ee/panel_msg.h>
#include <ee/FilepathDialog.h>
#include <ee/SymbolFile.h>

#include <sprite2/SymType.h>

namespace ecomplex
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, ee::EditPanelImpl* stage, ee::MultiSpritesImpl* sprites_impl, 
								 ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(wnd, stage, sprites_impl, callback)
	, m_open_symbol(wnd, stage, sprites_impl)
	, m_guides(NULL)
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
	else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'G')
	{
		StagePanel* stage = static_cast<StagePanel*>(m_wnd);
		const Symbol* parent = stage->GetSymbol();
		std::string dir = ee::FileHelper::GetFileDir(parent->GetFilepath());
		ecomplex::GroupHelper::BuildComplex(m_selection, dir, m_wnd);
		return true;
	}
	else if (m_stage->GetKeyState(WXK_ALT) && keyCode == 'B')
	{
		ecomplex::GroupHelper::BreakUpComplex(m_selection);
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
	if (m_guides && m_guides->OnMouseDown(m_stage->TransPosScrToProj(x, y))) {
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseLeftDown(x, y);
	}
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (m_guides && m_guides->OnMouseUp(m_stage->TransPosScrToProj(x, y))) {
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseLeftUp(x, y);
	}
}

bool SelectSpritesOP::OnMouseDrag(int x, int y)
{
	if (m_guides && m_guides->OnMouseDrag(m_stage->TransPosScrToProj(x, y))) {
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		return true;
	} else {
		return ee::SelectSpritesOP::OnMouseDrag(x, y);
	}
}

bool SelectSpritesOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_sprs_impl->QuerySpriteByPos(pos);
	if (selected) {
		m_open_symbol.Open(selected, m_guides);
	}

	return false;
}

bool SelectSpritesOP::OnDraw() const
{
	bool ret = ee::SelectSpritesOP::OnDraw();
	if (m_guides) {
		m_guides->Draw();
	}
	return ret;

}

} // complex