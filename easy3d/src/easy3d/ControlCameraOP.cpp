#include "ControlCameraOP.h"
#include "StageCanvas.h"

#include "RotateCameraState.h"
#include "TranslateCameraState.h"
#include "ZoomCameraState.h"

#include <ee/EditPanelImpl.h>

namespace e3d
{

static const float MOUSE_SENSITIVITY = 0.3f;

ControlCameraOP::ControlCameraOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::EditOP(wnd, stage)
{
	m_canvas = static_cast<e3d::StageCanvas*>(stage->GetCanvas());
}

bool ControlCameraOP::OnKeyDown(int keyCode)
{
	if (ee::EditOP::OnKeyDown(keyCode)) { return true; }

	static const float OFFSET = 0.1f;

	auto& cam = m_canvas->GetCameraUVN();
	switch (keyCode)
	{
	case WXK_ESCAPE:
		cam.Reset(sm::vec3(0, 0, -2), sm::vec3(0, 0, 0), sm::vec3(0, 1, 0));
		break;
	case 'w': case 'W':
		cam.Translate(0, OFFSET);
		m_canvas->Refresh();
		break;
	case 's': case 'S':
		cam.Translate(0, -OFFSET);
		m_canvas->Refresh();
		break;
 	case 'a': case 'A':
		cam.Translate(OFFSET, 0);
		m_canvas->Refresh();
 		break;
 	case 'd': case 'D':
		cam.Translate(-OFFSET, 0);
		m_canvas->Refresh();
 		break;
	}

	return false;
}

bool ControlCameraOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<RotateCameraState>(
		*m_canvas, m_canvas->GetCameraUVN(), sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool ControlCameraOP::OnMouseLeftUp(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<ZoomCameraState>(
		*m_canvas, m_canvas->GetCameraUVN());

	return false;
}

bool ControlCameraOP::OnMouseRightDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<TranslateCameraState>(
		*m_canvas, m_canvas->GetCameraUVN(), sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool ControlCameraOP::OnMouseRightUp(int x, int y)
{
	if (ee::EditOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<ZoomCameraState>(
		*m_canvas, m_canvas->GetCameraUVN());

	return false;
}

bool ControlCameraOP::OnMouseDrag(int x, int y)
{
	if (ee::EditOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (m_op_state) {
		m_op_state->OnMouseDrag(sm::vec2(x, y));
	}
	
	return false;
}

bool ControlCameraOP::OnMouseMove(int x, int y)
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

bool ControlCameraOP::OnMouseWheelRotation(int x, int y, int direction)
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