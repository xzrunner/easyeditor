#include "ZoomCameraState.h"

namespace esketch
{

ZoomCameraState::ZoomCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
{
}

void ZoomCameraState::OnMouseWheelRotation(const sm::ivec2& pos, int direction)
{
	e3d::Camera& cam = m_canvas->GetCamera3();
	cam.Zoom(direction > 0);
	m_canvas->Refresh();
}

}