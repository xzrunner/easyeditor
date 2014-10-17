#include "RotateCameraState.h"
#include "StageCanvas.h"

namespace libsketch
{

RotateCameraState::RotateCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
	, m_is_open(false)
{
}

void RotateCameraState::OnMousePress(const ivec2& pos)
{
	m_first_pos.x = pos.x;
	m_first_pos.y = pos.y;
	m_pre_ori = m_ori;

	m_is_open = true;
}

void RotateCameraState::OnMouseRelease(const ivec2& pos)
{
	m_is_open = false;
}

void RotateCameraState::OnMouseMove(const ivec2& pos)
{
	if (!m_is_open) { return; }

	e3d::Camera& cam = m_canvas->GetCamera3();

	vec3 start = cam.MapToSphere(m_first_pos);
	vec3 end = cam.MapToSphere(ivec2(pos.x, pos.y));
	Quaternion delta = Quaternion::CreateFromVectors(start, end);
	m_ori = delta.Rotated(m_pre_ori);

	cam.SetRotate(m_ori.ToMatrix());
	e3d::ShaderMgr::Instance()->SetModelView(cam.GetMatrix());

	m_canvas->Refresh();
}

}