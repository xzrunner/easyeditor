#include "CamControlOP.h"
#include "StageCanvas.h"

#include "CamRotateState.h"
#include "CamTranslateState.h"
#include "CamZoomState.h"

#include <ee/EditPanelImpl.h>

namespace enode3d
{

static const float MOUSE_SENSITIVITY = 0.3f;

CamControlOP::CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage)
	: ee::EditOP(wnd, stage)
{
	m_canvas = static_cast<enode3d::StageCanvas*>(stage->GetCanvas());
}

bool CamControlOP::OnKeyDown(int keyCode)
{
	if (ee::EditOP::OnKeyDown(keyCode)) { return true; }

	static const float OFFSET = 0.1f;

	auto& cam = m_canvas->GetCamera();
	switch (keyCode)
	{
	case WXK_ESCAPE:
		cam.Reset();
		m_canvas->Refresh();
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

bool CamControlOP::OnMouseLeftDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamRotateState>(
		*m_canvas, m_canvas->GetCamera(), sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_canvas->GetCamera());

	return false;
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (ee::EditOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamTranslateState>(
		*m_canvas, m_canvas->GetCamera(), sm::vec2(x, y));

	m_op_state->OnMousePress(sm::vec2(x, y));

	return false;
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (ee::EditOP::OnMouseRightUp(x, y)) {
		return true;
	}

	m_op_state = std::make_unique<CamZoomState>(
		*m_canvas, m_canvas->GetCamera());

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