#include "EditClipboxOP.h"

#include <ee/EditPanelImpl.h>
#include <ee/Math2D.h>
#include <ee/panel_msg.h>
#include <ee/EE_RVG.h>
#include <ee/style_config.h>

namespace eui
{

EditClipboxOP::EditClipboxOP(wxWindow* wnd, ee::EditPanelImpl* edit_impl,
							 ee::Rect& rect)
	: ee::ZoomViewOP(wnd, edit_impl, true)
	, m_rect(rect)
{
}

bool EditClipboxOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);

	m_clipbox_selected = -1;
	if (ee::Math2D::GetDistance(ee::Vector(m_rect.xmin, m_rect.ymin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 0;
	} else if (ee::Math2D::GetDistance(ee::Vector(m_rect.xmin, m_rect.ymax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 1;
	} else if (ee::Math2D::GetDistance(ee::Vector(m_rect.xmax, m_rect.ymax), pos) < NODE_RADIUS) {
		m_clipbox_selected = 2;
	} else if (ee::Math2D::GetDistance(ee::Vector(m_rect.xmax, m_rect.ymin), pos) < NODE_RADIUS) {
		m_clipbox_selected = 3;
	}

	return m_clipbox_selected != -1;
}

bool EditClipboxOP::OnMouseLeftUp(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	m_clipbox_selected = -1;

	return false;
}

bool EditClipboxOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseDrag(x, y)) return true;

	if (m_clipbox_selected == -1) {
		return false;
	}

	ee::Vector pos = m_stage->TransPosScrToProj(x, y);
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
	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return true;
}

bool EditClipboxOP::OnDraw() const
{
	if (ee::ZoomViewOP::OnDraw()) return true;

	ee::RVG::Rect(m_rect, ee::LIGHT_GREEN_LINE);

	ee::RVG::Circle(ee::Vector(m_rect.xmin, m_rect.ymin), NODE_RADIUS, true, ee::LIGHT_GREY);
	ee::RVG::Circle(ee::Vector(m_rect.xmin, m_rect.ymax), NODE_RADIUS, true, ee::LIGHT_GREY);
	ee::RVG::Circle(ee::Vector(m_rect.xmax, m_rect.ymax), NODE_RADIUS, true, ee::LIGHT_GREY);
	ee::RVG::Circle(ee::Vector(m_rect.xmax, m_rect.ymin), NODE_RADIUS, true, ee::LIGHT_GREY);

	return false;
}

}