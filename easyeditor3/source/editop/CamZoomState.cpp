#include "ee3/CamZoomState.h"

#include <ee/panel_msg.h>

#include <node3/Camera.h>
#include <node3/Viewport.h>

namespace ee3
{

CamZoomState::CamZoomState(n3::Camera& cam, const n3::Viewport& vp)
	: m_cam(cam)
	, m_vp(vp)
{
}

bool CamZoomState::OnMouseWheelRotation(int x, int y, int direction)
{
	sm::vec3 dir = m_vp.TransPos3ScreenToDir(sm::vec2(x, y), m_cam);
	static const float OFFSET = 0.05f;
	if (direction > 0) {
		m_cam.Move(dir, m_cam.GetDistance() * OFFSET);
	} else {
		m_cam.Move(dir, - m_cam.GetDistance() * OFFSET);
	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}