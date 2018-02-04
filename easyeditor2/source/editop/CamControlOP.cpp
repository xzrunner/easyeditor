#include "ee2/CamControlOP.h"
#include "ee2/CamZoomState.h"
#include "ee2/CamTranslateState.h"

#include <ee0/SubjectMgr.h>

#include <sprite2/Camera.h>

#include <wx/defs.h>

namespace ee2
{

CamControlOP::CamControlOP(s2::Camera& cam, ee0::SubjectMgr& sub_mgr, uint32_t flag)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
	, m_flag(flag)
{
	m_zoom_state      = std::make_shared<CamZoomState>(cam, sub_mgr);
	m_left_pan_state  = std::make_shared<CamTranslateState>(cam, sub_mgr);
	m_right_pan_state = std::make_shared<CamTranslateState>(cam, sub_mgr);
	m_op_state = m_zoom_state;
}

bool CamControlOP::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case WXK_SPACE:
		if (m_flag & LEFT_TAP) {
			ChangeEditOpState(m_left_pan_state);
		}
		break;
	case WXK_ESCAPE:
		m_cam.Reset();
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
	}

	return false;
}

bool CamControlOP::OnKeyUp(int keyCode)
{
	if ((m_flag & LEFT_TAP) && keyCode == WXK_SPACE) {
		ChangeEditOpState(m_zoom_state);
	}
	return false;
}

bool CamControlOP::OnMouseLeftDown(int x, int y)
{
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	return m_op_state->OnMouseRelease(x, y);	
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (m_flag & RIGHT_TAP) {
		ChangeEditOpState(m_right_pan_state);
	}
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (m_flag & RIGHT_TAP) {
		ChangeEditOpState(m_zoom_state);
	}
	return m_op_state->OnMouseRelease(x, y);
}

bool CamControlOP::OnMouseDrag(int x, int y)
{
	return m_op_state->OnMouseDrag(x, y);
}

bool CamControlOP::OnMouseMove(int x, int y)
{
	if (m_flag & MOUSE_MOVE_FOCUS) {
//		m_stage->SetFocus();
	}

	return false;
}

bool CamControlOP::OnMouseWheelRotation(int x, int y, int direction)
{
	return m_op_state->OnMouseWheelRotation(x, y, direction);
}

void CamControlOP::ChangeEditOpState(const ee0::EditOpStatePtr& state)
{
	if (m_op_state == state) {
		return;
	}
	
	if (m_op_state) {
		m_op_state->UnBind();
	}
	m_op_state = state;
	if (m_op_state) {
		m_op_state->Bind();
	}
}

}