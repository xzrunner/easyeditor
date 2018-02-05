#include "ee3/CamControlOP.h"
#include "ee3/CamRotateState.h"
#include "ee3/CamTranslateState.h"
#include "ee3/CamZoomState.h"

#include <ee0/MessageID.h>
#include <ee0/SubjectMgr.h>

#include <painting3/Camera.h>

#include <wx/defs.h>

namespace ee3
{

static const float MOUSE_SENSITIVITY = 0.3f;

CamControlOP::CamControlOP(pt3::Camera& cam, const pt3::Viewport& vp,
	                       ee0::SubjectMgr& sub_mgr)
	: ee0::EditOP()
	, m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_rotate_state    = std::make_shared<CamRotateState>(cam, sub_mgr);
	m_translate_state = std::make_shared<CamTranslateState>(cam, sub_mgr);
	m_zoom_state      = std::make_shared<CamZoomState>(cam, vp, sub_mgr);
	m_op_state = m_rotate_state;
}

bool CamControlOP::OnKeyDown(int keyCode)
{
	if (ee0::EditOP::OnKeyDown(keyCode)) { return true; }

	static const float OFFSET = 0.1f;

	switch (keyCode)
	{
	case WXK_ESCAPE:
		m_cam.Reset();
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
	case 'w': case 'W':
		m_cam.Translate(0, OFFSET);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
	case 's': case 'S':
		m_cam.Translate(0, -OFFSET);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
 	case 'a': case 'A':
		m_cam.Translate(OFFSET, 0);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
 		break;
 	case 'd': case 'D':
		m_cam.Translate(-OFFSET, 0);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
 		break;
	}

	return false;
}

bool CamControlOP::OnMouseLeftDown(int x, int y)
{
	if (ee0::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	ChangeEditOpState(m_rotate_state);
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	if (ee0::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	ChangeEditOpState(m_zoom_state);

	return false;
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (ee0::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	ChangeEditOpState(m_translate_state);
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (ee0::EditOP::OnMouseRightUp(x, y)) {
		return true;
	}

	ChangeEditOpState(m_zoom_state);

	return false;
}

bool CamControlOP::OnMouseDrag(int x, int y)
{
	if (ee0::EditOP::OnMouseDrag(x, y)) {
		return true;
	}

	return m_op_state->OnMouseDrag(x, y);
}

bool CamControlOP::OnMouseMove(int x, int y)
{
	if (ee0::EditOP::OnMouseMove(x, y)) {
		return true;
	}

	return m_op_state->OnMouseMove(x, y);
}

bool CamControlOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (ee0::EditOP::OnMouseWheelRotation(x, y, direction)) {
		return true;
	}

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