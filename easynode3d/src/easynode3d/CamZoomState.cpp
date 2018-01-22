#include "CamZoomState.h"
#include "StageCanvas.h"

#include <ee/panel_msg.h>

namespace enode3d
{

CamZoomState::CamZoomState(n3::Camera& cam, const n3::Viewport& vp)
	: m_cam(cam)
	, m_vp(vp)
{
}

void CamZoomState::OnMouseWheelRotation(int x, int y, int direction)
{
	sm::vec3 dir = m_vp.TransPos3ScreenToDir(sm::vec2(x, y), m_cam);
	static const float OFFSET = 0.1f;
	if (direction > 0) {
		m_cam.Move(dir, m_cam.GetDistance() * OFFSET);
	} else {
		m_cam.Move(dir, - m_cam.GetDistance() * OFFSET);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}