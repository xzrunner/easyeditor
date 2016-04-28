#include "ResizeBaseOP.h"
#include "ResizeAtomicOP.h"
#include "Symbol.h"

#include <ee/EditPanelImpl.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/SpriteRenderer.h>
#include <ee/EE_RVG.h>

namespace escale9
{

ResizeBaseOP::ResizeBaseOP(wxWindow* wnd, ee::EditPanelImpl* stage, Symbol* symbol)
	: ee::ZoomViewOP(wnd, stage, true)
	, m_symbol(symbol)
	, m_status(e_null)
{
}

bool ResizeBaseOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	if (!m_symbol) {
		return false;
	}

	m_firstPos = m_stage->TransPosScrToProj(x, y);

	const float hw = m_symbol->GetSize().Width() * 0.5f,
		hh = m_symbol->GetSize().Height() * 0.5f;
	if (ee::Math2D::IsPointInRect(m_firstPos, ee::Vector(-hw, -hh), REGION, REGION))
		m_status = e_leftlow;
	else if (ee::Math2D::IsPointInRect(m_firstPos, ee::Vector(hw, -hh), REGION, REGION))
		m_status = e_rightlow;
	else if (ee::Math2D::IsPointInRect(m_firstPos, ee::Vector(hw, hh), REGION, REGION))
		m_status = e_rightup;
	else if (ee::Math2D::IsPointInRect(m_firstPos, ee::Vector(-hw, hh), REGION, REGION))
		m_status = e_leftup;

	return false;
}

bool ResizeBaseOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (m_status != e_null)
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		const ee::Vector src(fabs(m_firstPos.x)*2, fabs(m_firstPos.y)*2),
			dst(fabs(pos.x)*2, fabs(pos.y)*2);
		ee::EditAddRecordSJ::Instance()->Add(new ResizeAtomicOP(m_symbol, src, dst));
	}

	m_status = e_null;


	return false;
}

bool ResizeBaseOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_status != e_null)
	{
		ee::Vector pos = m_stage->TransPosScrToProj(x, y);
		m_symbol->ResizeScale9(fabs(pos.x)*2, fabs(pos.y)*2);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool ResizeBaseOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::SpriteRenderer::Draw(m_symbol);

	const float hw = m_symbol->GetSize().Width() * 0.5f,
		hh = m_symbol->GetSize().Height() * 0.5f;
	const float r = REGION;
	ee::RVG::Rect(ee::Vector(-hw, -hh), r, r, false);
	ee::RVG::Rect(ee::Vector( hw, -hh), r, r, false);
	ee::RVG::Rect(ee::Vector( hw,  hh), r, r, false);
	ee::RVG::Rect(ee::Vector(-hw,  hh), r, r, false);

	return false;
}

} // escale9