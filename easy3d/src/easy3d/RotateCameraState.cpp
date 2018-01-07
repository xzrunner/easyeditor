#include "RotateCameraState.h"
#include "StageCanvas.h"

#include <SM_Calc.h>
#include <node3/Camera.h>

namespace e3d
{

RotateCameraState::RotateCameraState(StageCanvas& canvas, 
	                                 n3::Camera& cam,
	                                 const sm::vec2& pos)
	: m_canvas(canvas)
	, m_cam(cam)
	, m_last_pos(pos)
{
}

void RotateCameraState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void RotateCameraState::OnMouseRelease(const sm::vec2& pos)
{
}

void RotateCameraState::RotateCameraState::OnMouseDrag(const sm::vec2& pos)
{
	float dx = pos.x - m_last_pos.x;
	float dy = pos.y - m_last_pos.y;

	m_cam.Yaw(dx);
	m_cam.Pitch(dy);
	m_cam.AimAtTarget();

	m_last_pos = pos;

	m_canvas.Refresh();
}

}