#include "ee3/CamRotateState.h"

#include <ee/panel_msg.h>

#include <SM_Calc.h>
#include <node3/Camera.h>

namespace ee3
{

CamRotateState::CamRotateState(n3::Camera& cam)
	: m_cam(cam)
{
	m_last_pos.MakeInvalid();
}

bool CamRotateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool CamRotateState::OnMouseRelease(int x, int y)
{
	return false;
}

bool CamRotateState::CamRotateState::OnMouseDrag(int x, int y)
{
	int dx = x - m_last_pos.x;
	int dy = y - m_last_pos.y;

	m_cam.Yaw(- dx * 0.2f);
	m_cam.Pitch(- dy * 0.2f);
	m_cam.AimAtTarget();
	m_cam.SetUpDir(sm::vec3(0, 1, 0));

	m_last_pos.Set(x, y);

	ee::SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

}