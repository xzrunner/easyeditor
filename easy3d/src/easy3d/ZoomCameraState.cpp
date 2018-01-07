#include "ZoomCameraState.h"
#include "StageCanvas.h"

namespace e3d
{

ZoomCameraState::ZoomCameraState(StageCanvas& canvas, n3::Camera& cam)
	: m_canvas(canvas)
	, m_cam(cam)
{
}

void ZoomCameraState::OnMouseWheelRotation(int x, int y, int direction)
{
	static const float OFFSET = 0.1f;
	if (direction > 0) {
		m_cam.MoveToward(-OFFSET);
	} else {
		m_cam.MoveToward(OFFSET);
	}
	m_canvas.Refresh();
}

}