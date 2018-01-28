#include "ee3/CamTranslateState.h"

#include <ee/panel_msg.h>

#include <node3/Camera.h>

namespace ee3
{

CamTranslateState::CamTranslateState(n3::Camera& cam)
	: m_cam(cam)
{
	m_last_pos.MakeInvalid();
}

bool CamTranslateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool CamTranslateState::OnMouseRelease(int x, int y)
{
	return false;
}

bool CamTranslateState::CamTranslateState::OnMouseDrag(int x, int y)
{
	int dx = x - m_last_pos.x;
	int dy = y - m_last_pos.y;

	float dist = m_cam.GetDistance();
	static const float SCALE = 0.002f;
	m_cam.Translate(dx * dist * SCALE, dy * dist * SCALE);

	m_last_pos.Set(x, y);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}