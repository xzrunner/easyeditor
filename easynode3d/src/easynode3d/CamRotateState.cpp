#include "CamRotateState.h"
#include "StageCanvas.h"

#include <SM_Calc.h>
#include <node3/Camera.h>

namespace enode3d
{

CamRotateState::CamRotateState(StageCanvas& canvas, 
	                                 n3::Camera& cam,
	                                 const sm::vec2& pos)
	: m_canvas(canvas)
	, m_cam(cam)
	, m_last_pos(pos)
{
}

void CamRotateState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void CamRotateState::OnMouseRelease(const sm::vec2& pos)
{
}

void CamRotateState::CamRotateState::OnMouseDrag(const sm::vec2& pos)
{
	float dx = pos.x - m_last_pos.x;
	float dy = pos.y - m_last_pos.y;

	m_cam.Yaw(- dx * 0.2f);
	m_cam.Pitch(- dy * 0.2f);
	m_cam.AimAtTarget();
	m_cam.SetUpDir(sm::vec3(0, 1, 0));

	m_last_pos = pos;

	m_canvas.Refresh();
}

}