#include "ee3/CamTranslateState.h"

#include <ee0/SubjectMgr.h>

#include <painting3/Camera.h>

namespace ee3
{

CamTranslateState::CamTranslateState(pt3::Camera& cam, ee0::SubjectMgr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
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

bool CamTranslateState::OnMouseDrag(int x, int y)
{
	int dx = x - m_last_pos.x;
	int dy = y - m_last_pos.y;

	float dist = m_cam.GetDistance();
	static const float SCALE = 0.002f;
	m_cam.Translate(dx * dist * SCALE, dy * dist * SCALE);

	m_last_pos.Set(x, y);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

}