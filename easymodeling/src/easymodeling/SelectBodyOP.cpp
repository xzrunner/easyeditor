#include "SelectBodyOP.h"
#include "BodyPropertySetting.h"
#include "WorldPropertySetting.h"
#include "DrawUtils.h"
#include "StagePanel.h"
#include "BodyEditStage.h"
#include "Context.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/SpriteSelection.h>

namespace emodeling
{

SelectBodyOP::SelectBodyOP(wxWindow* stage_wnd,
						   ee::EditPanelImpl* stage, 
						   ee::MultiSpritesImpl* sprites_impl, 
						   ee::EditCMPT* callback/* = NULL*/)
	: ee::SelectSpritesOP(stage_wnd, stage, sprites_impl, callback)
	, m_mouseOn(NULL)
{
}

bool SelectBodyOP::OnKeyDown(int keyCode)
{
	if (ee::SelectSpritesOP::OnKeyDown(keyCode)) return true;

	if (keyCode == WXK_DELETE && m_mouseOn)
		m_mouseOn = NULL;

	return false;
}

bool SelectBodyOP::OnMouseMove(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseMove(x, y)) return true;

	m_mouseOn = NULL;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	ee::Sprite* selected = m_spritesImpl->QuerySpriteByPos(pos);
	if (selected)
		m_mouseOn = static_cast<Body*>(selected->GetUserData());

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool SelectBodyOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::SelectSpritesOP::OnMouseLeftDClick(x, y)) return true;

	// todo

// 	sm::vec2 pos = m_stage->transPosScreenToProject(x, y);
// 	ee::Sprite* selected = m_spritesImpl->querySpriteByPos(pos);
// 	if (selected)
// 	{
// 		eshape::Symbol symbol;
// 		symbol.SetBG(&const_cast<ee::Symbol&>(selected->getSymbol()));
// 		eshape::EditDialog<BodyEditStage> dlg(Context::Instance()->stage, &symbol);
// 		dlg.ShowModal();
// 		Context::Instance()->stage->resetCanvas();
// 	}

	return false;
}

bool SelectBodyOP::OnDraw() const
{
	if (ee::SelectSpritesOP::OnDraw()) return true;

	m_selection->Traverse(DrawSelectedVisitor());

	if (m_mouseOn)
		DrawUtils::DrawBody(m_mouseOn, DrawUtils::e_mouseOn);

	return false;
}

bool SelectBodyOP::Clear()
{
	if (ee::SelectSpritesOP::Clear()) return true;

	m_mouseOn = NULL;

	return false;
}

//////////////////////////////////////////////////////////////////////////
// class SelectBodyOP::DrawSelectedVisitor
//////////////////////////////////////////////////////////////////////////

void SelectBodyOP::DrawSelectedVisitor::
Visit(ee::Sprite* spr, bool& next) 
{
	std::vector<sm::vec2> bound;
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	Body* body = static_cast<Body*>(sprite->GetUserData());
	DrawUtils::DrawBody(body, DrawUtils::e_selected);
	next = true;
}

}