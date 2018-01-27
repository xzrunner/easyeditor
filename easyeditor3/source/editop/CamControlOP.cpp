#include "ee3/CamControlOP.h"

#include "ee3/CamRotateState.h"
#include "ee3/CamTranslateState.h"
#include "ee3/CamZoomState.h"

#include <ee0/MessageID.h>
#include <ee0/SubjectMgr.h>

#include <node3/Camera.h>

namespace ee3
{

static const float MOUSE_SENSITIVITY = 0.3f;

CamControlOP::CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
	                       n3::Camera& cam, const n3::Viewport& vp,
	                       ee0::SubjectMgr& sub_mgr)
	: ee::EditOP(wnd, stage)
	, m_cam(cam)
	, m_vp(vp)
	, m_sub_mgr(sub_mgr)
{
}

bool CamControlOP::OnKeyDown(int keyCode)
{
	if (ee::EditOP::OnKeyDown(keyCode)) { return true; }

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
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamRotateState>(m_cam, sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(m_cam, m_vp);

	return false;
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamTranslateState>(m_cam, sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (ee::EditOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(m_cam, m_vp);

	return false;
}

bool CamControlOP::OnMouseDrag(int x, int y)
{
	if (ee::EditOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseDrag(sm::vec2(x, y));
	}
	
	return false;
}

bool CamControlOP::OnMouseMove(int x, int y)
{
	if (ee::EditOP::OnMouseMove(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseMove(sm::vec2(x, y));
	}

//	m_stage->SetFocus();

	return false;
}

bool CamControlOP::OnMouseWheelRotation(int x, int y, int direction)
{
	if (ee::EditOP::OnMouseWheelRotation(x, y, direction)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseWheelRotation(x, y, direction);
	}

	return false;
}

}