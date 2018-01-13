#include "CamZoomState.h"
#include "StageCanvas.h"

namespace enode3d
{

CamZoomState::CamZoomState(StageCanvas& canvas, n3::Camera& cam)
	: m_canvas(canvas)
	, m_cam(cam)
{
}

void CamZoomState::OnMouseWheelRotation(int x, int y, int direction)
{
	sm::vec3 dir = m_canvas.TransPos3ScreenToDir(sm::vec2(x, y));
	static const float OFFSET = 0.1f;
	if (direction > 0) {
		m_cam.MoveToward(dir, m_cam.GetDistance() * OFFSET);
	} else {
		m_cam.MoveToward(dir, - m_cam.GetDistance() * OFFSET);
	}

	m_canvas.Refresh();
}

}