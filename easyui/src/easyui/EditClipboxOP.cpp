#include "EditClipboxOP.h"

namespace eui
{

EditClipboxOP::EditClipboxOP(wxWindow* wnd, d2d::EditPanelImpl* edit_impl,
							 d2d::Rect& rect)
	: d2d::ZoomViewOP(wnd, edit_impl, true)
	, m_rect(rect)
{
}

bool EditClipboxOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);

	m_clipbox_selected = -1;
	if (d2d::Math2D::GetDistance(d2d::Vector(m_rect.xmin, m_rect.ymin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 0;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(m_rect.xmin, m_rect.ymax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 1;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(m_rect.xmax, m_rect.ymax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 2;
	} else if (d2d::Math2D::GetDistance(d2d::Vector(m_rect.xmax, m_rect.ymin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 3;
	}

	return m_clipbox_selected != -1;
}

bool EditClipboxOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_clipbox_selected = -1;

	return false;
}

bool EditClipboxOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_clipbox_selected == -1) {
		return false;
	}

	d2d::Vector pos = m_stage->TransPosScrToProj(x, y);
	if (m_clipbox_selected == 0) {
		m_rect.xmin = std::min(m_rect.xmax, pos.x);			
		m_rect.ymin = std::min(m_rect.ymax, pos.y);
	} else if (m_clipbox_selected == 1) {
		m_rect.xmin = std::min(m_rect.xmax, pos.x);
		m_rect.ymax = std::max(m_rect.ymin, pos.y);
	} else if (m_clipbox_selected == 2) {
		m_rect.xmax = std::max(m_rect.xmin, pos.x);
		m_rect.ymax = std::max(m_rect.ymin, pos.y);
	} else if (m_clipbox_selected == 3) {
		m_rect.xmax = std::max(m_rect.xmin, pos.x);
		m_rect.ymin = std::min(m_rect.ymax, pos.y);
	}
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	return true;
}

bool EditClipboxOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	d2d::PrimitiveDraw::DrawRect(m_rect, d2d::LIGHT_GREEN_LINE);

	d2d::PrimitiveDraw::DrawCircle(d2d::Vector(m_rect.xmin, m_rect.ymin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::DrawCircle(d2d::Vector(m_rect.xmin, m_rect.ymax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::DrawCircle(d2d::Vector(m_rect.xmax, m_rect.ymax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::DrawCircle(d2d::Vector(m_rect.xmax, m_rect.ymin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);

	return false;
}

}