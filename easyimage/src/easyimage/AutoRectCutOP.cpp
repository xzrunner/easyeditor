#include "AutoRectCutOP.h"
#include "StagePanel.h"

#include <ee/EditPanelImpl.h>
#include <ee/Rect.h>
#include <ee/panel_msg.h>
#include <ee/PrimitiveDraw.h>
#include <ee/style_config.h>

namespace eimage
{

AutoRectCutOP::AutoRectCutOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::ZoomViewOP(wnd, stage, true, false, false)
	, m_selected(NULL)
{
}

bool AutoRectCutOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_last_pos = m_stage->TransPosScrToProj(x, y);
	m_selected = m_rects.QueryRect(m_last_pos);

	return false;
}

bool AutoRectCutOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	if (m_selected) {
		m_selected->xmin = (int)(m_selected->xmin+0.5f);
		m_selected->xmax = (int)(m_selected->xmax+0.5f);
		m_selected->ymin = (int)(m_selected->ymin+0.5f);
		m_selected->ymax = (int)(m_selected->ymax+0.5f);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool AutoRectCutOP::OnMouseRightDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	bool removed = m_rects.Remove(pos);
	if (removed) {
		m_selected = NULL;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool AutoRectCutOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_selected)
	{
		ee::Vector curr = m_stage->TransPosScrToProj(x, y);
		m_rects.MoveRect(m_selected, m_last_pos, curr);
		m_last_pos = curr;
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}

	return false;
}

bool AutoRectCutOP::OnMouseLeftDClick(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDClick(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
	ee::Rect* r = m_rects.QueryRect(pos);
	if (r) {
		m_rects.Insert(ee::Rect(*r), true);
		m_last_pos = pos;
		m_selected = r;
	}

	return false;
}

bool AutoRectCutOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::PrimitiveDraw::Cross(ee::Vector(0, 0), 100, 100, ee::Colorf(1, 0, 0));

	m_rects.Draw();

	if (m_selected) {
		ee::PrimitiveDraw::DrawRect(*m_selected, ee::LIGHT_GREEN_FACE);
	}

	return false;
}

bool AutoRectCutOP::Clear()
{
	if (ee::ZoomViewOP::Clear()) return true;

	m_rects.Clear();

	return false;
}

}