#include "ControlCameraOP.h"
#include "StageCanvas.h"

namespace e3d
{

ControlCameraOP::ControlCameraOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
{
	m_canvas = static_cast<e3d::StageCanvas*>(stage->getCanvas());
}

bool ControlCameraOP::onKeyDown(int keyCode)
{
	if (d2d::AbstractEditOP::onKeyDown(keyCode)) { return true; }

	switch (keyCode)
	{
// 	case 't': case 'T':
// 		{
// 			mat4 mat = mat4::RotateX(-60);
// 			vec3 dir = mat * vec3(0, 0, -1);
// 			vec3 dir2 = mat * vec3(0, 0, 1);
// 			int zz = 0;
// 		}
// 		break;
	case WXK_SPACE:
		m_canvas->GetCamera3().Reset();
		m_canvas->Refresh();
		break;
	case 'w': case 'W':
		{
			Camera& cam = m_canvas->GetCamera3();
			vec3 offset = cam.GetRotate() * vec3(0, 0, -1);
			m_canvas->GetCamera3().Translate(offset);
			m_canvas->Refresh();
		}
		break;
	case 's': case 'S':
		{
			Camera& cam = m_canvas->GetCamera3();
			vec3 offset = cam.GetRotate() * vec3(0, 0, 1);
			m_canvas->GetCamera3().Translate(offset);
			m_canvas->Refresh();
		}
		break;
	case 'a': case 'A':
		{
			Camera& cam = m_canvas->GetCamera3();
			vec3 offset = cam.GetRotate() * vec3(-1, 0, 0);
			m_canvas->GetCamera3().Translate(offset);
			m_canvas->Refresh();
		}
		break;
	case 'd': case 'D':
		{
			Camera& cam = m_canvas->GetCamera3();
			vec3 offset = cam.GetRotate() * vec3(1, 0, 0);
			m_canvas->GetCamera3().Translate(offset);
			m_canvas->Refresh();
		}
		break;
	}

	return false;
}

bool ControlCameraOP::onMouseMove(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseMove(x, y)) {
		return true;
	}

	m_editPanel->SetFocus();

	return false;
}

}