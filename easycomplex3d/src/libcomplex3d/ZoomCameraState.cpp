#include "ZoomCameraState.h"

#include <easy3d/Camera.h>
#include <easy3d/StageCanvas.h>

namespace ecomplex3d
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