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
	if (d2d::Math::getDistance(d2d::Vector(m_rect.xMin, m_rect.yMin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 0;
	} else if (d2d::Math::getDistance(d2d::Vector(m_rect.xMin, m_rect.yMax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 1;
	} else if (d2d::Math::getDistance(d2d::Vector(m_rect.xMax, m_rect.yMax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 2;
	} else if (d2d::Math::getDistance(d2d::Vector(m_rect.xMax, m_rect.yMin), pos) < NODE_RADIUS) {
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
		m_rect.xMin = std::min(m_rect.xMax, pos.x);			
		m_rect.yMin = std::min(m_rect.yMax, pos.y);
	} else if (m_clipbox_selected == 1) {
		m_rect.xMin = std::min(m_rect.xMax, pos.x);
		m_rect.yMax = std::max(m_rect.yMin, pos.y);
	} else if (m_clipbox_selected == 2) {
		m_rect.xMax = std::max(m_rect.xMin, pos.x);
		m_rect.yMax = std::max(m_rect.yMin, pos.y);
	} else if (m_clipbox_selected == 3) {
		m_rect.xMax = std::max(m_rect.xMin, pos.x);
		m_rect.yMin = std::min(m_rect.yMax, pos.y);
	}
	m_stage->SetCanvasDirty();

	return true;
}

bool EditClipboxOP::OnDraw() const
{
	if (d2d::ZoomViewOP::OnDraw()) return true;

	d2d::PrimitiveDraw::rect(m_rect, d2d::LIGHT_GREEN_LINE);

	d2d::PrimitiveDraw::drawCircle(d2d::Vector(m_rect.xMin, m_rect.yMin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(m_rect.xMin, m_rect.yMax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(m_rect.xMax, m_rect.yMax), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);
	d2d::PrimitiveDraw::drawCircle(d2d::Vector(m_rect.xMax, m_rect.yMin), NODE_RADIUS, true, 2, d2d::LIGHT_GREY);

	return false;
}

}