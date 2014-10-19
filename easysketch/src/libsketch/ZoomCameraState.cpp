#include "ZoomCameraState.h"

namespace libsketch
{

ZoomCameraState::ZoomCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
{
}

void ZoomCameraState::OnMouseWheelRotation(const ivec2& pos, int direction)
{
	e3d::Camera& cam = m_canvas->GetCamera3();
	cam.Zoom(direction > 0);
	m_canvas->Refresh();
}

}