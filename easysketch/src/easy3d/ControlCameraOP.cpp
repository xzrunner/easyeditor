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
	case WXK_SPACE:
		m_canvas->GetCamera3().Reset();
		m_canvas->Refresh();
		break;
	case 'w': case 'W':
		m_canvas->GetCamera3().Zoom(true);
		m_canvas->Refresh();
		break;
	case 's': case 'S':
		m_canvas->GetCamera3().Zoom(false);
		m_canvas->Refresh();
		break;
	case 'a': case 'A':
		m_canvas->GetCamera3().Translate(vec3(-1, 0, 0));
		m_canvas->Refresh();
		break;
	case 'd': case 'D':
		m_canvas->GetCamera3().Translate(vec3(1, 0, 0));
		m_canvas->Refresh();
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