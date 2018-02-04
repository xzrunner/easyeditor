#include "ee2/DrawSelectRectState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/CameraHelper.h>
#include <ee0/color_config.h>

#include <sprite2/RVG.h>

namespace ee2
{

DrawSelectRectState::DrawSelectRectState(s2::Camera& cam, ee0::SubjectMgr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawSelectRectState::OnMousePress(int x, int y)
{
	m_first_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	return false;
}

bool DrawSelectRectState::OnMouseRelease(int x, int y)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

bool DrawSelectRectState::OnMouseDrag(int x, int y)
{
	if (m_first_pos.IsValid())
	{
		m_last_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}

	return false;
}

bool DrawSelectRectState::OnDraw() const
{
	if (!m_first_pos.IsValid() || !m_last_pos.IsValid()) {
		return false;
	}

	s2::RVG::LineWidth(2);
	if (m_last_pos.x > m_first_pos.x)
	{
		s2::RVG::SetColor(ee0::LIGHT_GREEN);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(ee0::SELECT_RED);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, false);
	}
	else
	{
		s2::RVG::SetColor(ee0::LIGHT_BLUE);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, true);
		s2::RVG::SetColor(ee0::SELECT_RED);
		s2::RVG::Rect(nullptr, m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawSelectRectState::Clear()
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}