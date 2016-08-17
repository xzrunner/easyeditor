#include "ResizeBaseOP.h"
#include "ResizeAtomicOP.h"
#include "Symbol.h"

#include <ee/EditPanelImpl.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>

#include <sprite2/S2_RVG.h>
#include <SM_Test.h>

namespace escale9
{

ResizeBaseOP::ResizeBaseOP(wxWindow* wnd, ee::EditPanelImpl* stage, Symbol* sym)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_sym(sym)
	, m_status(e_null)
{
}

bool ResizeBaseOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_sym) {
		return false;
	}

	m_firstPos = m_stage->TransPosScrToProj(x, y);

	sm::vec2 sz = m_sym->GetBounding().Size();
	const float hw = sz.x * 0.5f, hh = sz.y * 0.5f;
	if (sm::is_point_in_rect(m_firstPos, sm::rect(sm::vec2(-hw, -hh), REGION, REGION)))
		m_status = e_leftlow;
	else if (sm::is_point_in_rect(m_firstPos, sm::rect(sm::vec2(hw, -hh), REGION, REGION)))
		m_status = e_rightlow;
	else if (sm::is_point_in_rect(m_firstPos, sm::rect(sm::vec2(hw, hh), REGION, REGION)))
		m_status = e_rightup;
	else if (sm::is_point_in_rect(m_firstPos, sm::rect(sm::vec2(-hw, hh), REGION, REGION)))
		m_status = e_leftup;

	return false;
}

bool ResizeBaseOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (m_status != e_null)
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		const sm::vec2 src(fabs(m_firstPos.x)*2, fabs(m_firstPos.y)*2),
			dst(fabs(pos.x)*2, fabs(pos.y)*2);
		ee::EditAddRecordSJ::Instance()->Add(new ResizeAtomicOP(m_sym, src, dst));
	}

	m_status = e_null;


	return false;
}

bool ResizeBaseOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_status != e_null)
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		m_sym->ResizeScale9(fabs(pos.x)*2, fabs(pos.y)*2);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool ResizeBaseOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::SpriteRenderer::Draw(m_sym);
	
	sm::vec2 sz = m_sym->GetBounding().Size();
	const float hw = sz.x * 0.5f, hh = sz.y * 0.5f;
	const float r = REGION;
	s2::RVG::Rect(sm::vec2(-hw, -hh), r, r, false);
	s2::RVG::Rect(sm::vec2( hw, -hh), r, r, false);
	s2::RVG::Rect(sm::vec2( hw,  hh), r, r, false);
	s2::RVG::Rect(sm::vec2(-hw,  hh), r, r, false);

	return false;
}

} // escale9