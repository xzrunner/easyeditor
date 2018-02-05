#include "ee3/CamRotateState.h"

#include <ee0/SubjectMgr.h>

#include <SM_Calc.h>
#include <painting3/Camera.h>

namespace ee3
{

CamRotateState::CamRotateState(pt3::Camera& cam, ee0::SubjectMgr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
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

bool CamRotateState::OnMouseDrag(int x, int y)
{
	int dx = x - m_last_pos.x;
	int dy = y - m_last_pos.y;

	m_cam.Yaw(- dx * 0.2f);
	m_cam.Pitch(- dy * 0.2f);
	m_cam.AimAtTarget();
	m_cam.SetUpDir(sm::vec3(0, 1, 0));

	m_last_pos.Set(x, y);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

}