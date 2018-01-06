#include "RotateCameraState.h"
#include "Camera.h"
#include "StageCanvas.h"

namespace e3d
{

RotateCameraState::RotateCameraState(StageCanvas& canvas, Camera& cam,
	                                 const sm::ivec2& pos)
	: m_canvas(canvas)
	, m_cam(cam)
	, m_last_pos(pos)
{
}

void RotateCameraState::OnMousePress(const sm::ivec2& pos)
{
	m_last_pos = pos;
}

void RotateCameraState::OnMouseRelease(const sm::ivec2& pos)
{
}

void RotateCameraState::RotateCameraState::OnMouseDrag(const sm::ivec2& pos)
{
	// todo cam's center
	sm::ivec2 center = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));
	sm::ivec2 base = m_canvas.TransPos3ProjectToScreen(sm::vec3(0, 0, 0));

	sm::vec3 start = m_cam.MapToSphere(base + m_last_pos - center);
	sm::vec3 end = m_cam.MapToSphere(base + pos - center);
	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);

	m_last_pos = pos;

	m_canvas.Refresh();
}

}