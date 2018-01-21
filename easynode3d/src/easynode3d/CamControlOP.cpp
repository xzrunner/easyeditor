#include "CamControlOP.h"

#include "CamRotateState.h"
#include "CamTranslateState.h"
#include "CamZoomState.h"

#include <ee/EditPanelImpl.h>
#include <ee/StageCanvas.h>

#include <node3/Camera.h>

namespace enode3d
{

static const float MOUSE_SENSITIVITY = 0.3f;

CamControlOP::CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage, 
	                       n3::Camera& cam, const n3::Viewport& vp)
	: ee::EditOP(wnd, stage)
	, m_canvas(stage->GetCanvas())
	, m_cam(cam)
	, m_vp(vp)
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
		m_canvas->RefreshCanvas();
		break;
	case 'w': case 'W':
		m_cam.Translate(0, OFFSET);
		m_canvas->RefreshCanvas();
		break;
	case 's': case 'S':
		m_cam.Translate(0, -OFFSET);
		m_canvas->RefreshCanvas();
		break;
 	case 'a': case 'A':
		m_cam.Translate(OFFSET, 0);
		m_canvas->RefreshCanvas();
 		break;
 	case 'd': case 'D':
		m_cam.Translate(-OFFSET, 0);
		m_canvas->RefreshCanvas();
 		break;
	}

	return false;
}

bool CamControlOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamRotateState>(
		*m_canvas, m_cam, sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_cam, m_vp);

	return false;
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamTranslateState>(
		*m_canvas, m_cam, sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (ee::EditOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_cam, m_vp);

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