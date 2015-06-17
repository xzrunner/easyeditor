#include "ControlCameraOP.h"
#include "StageCanvas.h"

namespace e3d
{

static const float MOUSE_SENSITIVITY = 0.3f;

ControlCameraOP::ControlCameraOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
{
	m_canvas = static_cast<e3d::StageCanvas*>(stage->getCanvas());
}

bool ControlCameraOP::OnKeyDown(int keyCode)
{
	if (d2d::AbstractEditOP::OnKeyDown(keyCode)) { return true; }

	Camera& cam = m_canvas->GetCamera3();
	switch (keyCode)
	{
	case WXK_SPACE:
		cam.Reset();
		m_canvas->Refresh();
		break;
	case 'w': case 'W':
		cam.Translate(cam.GetToward());
		m_canvas->Refresh();
		break;
	case 's': case 'S':
		cam.Translate(-cam.GetToward());
		m_canvas->Refresh();
		break;
 	case 'a': case 'A':
		cam.Translate(cam.GetLeft());
		m_canvas->Refresh();
 		break;
 	case 'd': case 'D':
		cam.Translate(-cam.GetLeft());
		m_canvas->Refresh();
 		break;
	}

	return false;
}

bool ControlCameraOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseLeftDown(x, y)) {
		return true;
	}
	m_last_pos.x = x;
	m_last_pos.y = y;
	return false;
}

bool ControlCameraOP::OnMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseDrag(x, y)) {
		return true;
	}

	Camera& cam = m_canvas->GetCamera3();
	float dx = (x - m_last_pos.x) * MOUSE_SENSITIVITY,
		  dy = (m_last_pos.y - y) * MOUSE_SENSITIVITY;
	cam.Rotate(dx, dy);
	m_canvas->Refresh();

	m_last_pos.x = x;
	m_last_pos.y = y;

	return false;
}

bool ControlCameraOP::OnMouseMove(int x, int y)
{
	if (d2d::AbstractEditOP::OnMouseMove(x, y)) {
		return true;
	}
	m_stage->SetFocus();
	return false;
}

}