#include "ZoomViewOP.h"
#include "ZoomViewState.h"
#include "PanViewState.h"

#include "view/Camera.h"
#include "view/EditPanelImpl.h"
#include "common/config.h"
#include "common/SettingData.h"

namespace d2d
{

ZoomViewOP::ZoomViewOP(wxWindow* wnd, EditPanelImpl* stage, 
					   bool mouse_move_focus, bool right_tap, bool left_tap) 
	: AbstractEditOP(wnd, stage)
	, m_mouse_move_focus(mouse_move_focus)
	, m_open_right_pan(right_tap)
	, m_open_left_pan(left_tap)
{
	const SettingData& setting = Config::Instance()->GetSettings();
	m_mouse_move_focus = setting.auto_get_focus;

	m_view_state = new ZoomViewState(stage);
	m_left_pan_state = new PanViewState(stage);
	m_right_pan_state = new PanViewState(stage);

	m_op_state = m_view_state;
}

ZoomViewOP::~ZoomViewOP()
{
	delete m_view_state;
	delete m_left_pan_state;
	delete m_right_pan_state;
}

bool ZoomViewOP::OnKeyDown(int keyCode)
{
	switch (keyCode)
	{
	case WXK_SPACE:
		if (m_open_left_pan) {
			SwitchState(m_left_pan_state);
		}
		break;
	case WXK_ESCAPE:
		{
			Camera* cam = m_stage->GetCamera();
			cam->Reset();
		}
		break;
	}

	return false;
}

bool ZoomViewOP::OnKeyUp(int keyCode)
{
	if (m_open_left_pan && keyCode == WXK_SPACE) {
		SwitchState(m_view_state);
	}
	return false;
}

bool ZoomViewOP::OnMouseLeftDown(int x, int y)
{
	return m_op_state->OnMousePress(x, y);
}

bool ZoomViewOP::OnMouseLeftUp(int x, int y)
{
	return m_op_state->OnMouseRelease(x, y);
}

bool ZoomViewOP::OnMouseRightDown(int x, int y) 
{ 
	if (m_open_right_pan) {
		SwitchState(m_right_pan_state);
	}
	return m_op_state->OnMousePress(x, y);
}

bool ZoomViewOP::OnMouseRightUp(int x, int y) 
{ 
	if (m_open_right_pan) {
		SwitchState(m_view_state);
	}
	return m_op_state->OnMouseRelease(x, y);
}

bool ZoomViewOP::OnMouseMove(int x, int y) 
{
	if (m_mouse_move_focus) {
		m_stage->SetFocus();
	}

	return false;
}

bool ZoomViewOP::OnMouseDrag(int x, int y)
{
	return m_op_state->OnMouseDrag(x, y);
}

bool ZoomViewOP::OnMouseWheelRotation(int x, int y, int direction) 
{
	return m_op_state->OnMouseWheelRotation(x, y, direction);
}

void ZoomViewOP::SwitchState(IEditOPState* state)
{
	if (state == m_op_state) {
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

} // d2d