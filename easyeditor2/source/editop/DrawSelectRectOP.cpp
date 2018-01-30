#include "ee2/DrawSelectRectOP.h"

#include <ee/color_config.h>

#include <ee0/SubjectMgr.h>
#include <ee0/CameraHelper.h>

#include <sprite2/RVG.h>

namespace ee2
{

DrawSelectRectOP::DrawSelectRectOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
	                               s2::Camera& cam, ee0::SubjectMgr& sub_mgr, 
                                   bool open_right_tap)
	: CamControlOP(wnd, stage, cam, sub_mgr, CamControlOP::DEFAULT_FLAG | CamControlOP::RIGHT_TAP * open_right_tap)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawSelectRectOP::OnMouseLeftDown(int x, int y)
{
	if (CamControlOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_first_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	return false;
}

bool DrawSelectRectOP::OnMouseLeftUp(int x, int y)
{
	if (CamControlOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

bool DrawSelectRectOP::OnMouseDrag(int x, int y)
{
	if (CamControlOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_first_pos.IsValid())
	{
		m_last_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}

	return false;
}

bool DrawSelectRectOP::OnDraw() const
{
	if (CamControlOP::OnDraw()) {
		return true;
	}

	if (!m_first_pos.IsValid() || !m_last_pos.IsValid()) {
		return false;
	}

	s2::RVG::LineWidth(2);
	if (m_last_pos.x > m_first_pos.x)
	{
		s2::RVG::SetColor(ee::LIGHT_GREEN);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(ee::SELECT_RED);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, false);
	}
	else
	{
		s2::RVG::SetColor(ee::LIGHT_BLUE);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(ee::SELECT_RED);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawSelectRectOP::Clear()
{
	if (CamControlOP::Clear()) {
		return true;
	}

	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}