#include "RotateSpriteState.h"
#include "Sprite.h"

namespace libsketch
{

RotateSpriteState::RotateSpriteState(e3d::StageCanvas* canvas,
									 Sprite* sprite)
	: m_canvas(canvas)
	, m_sprite(sprite)
	, m_is_open(false)
{
}

void RotateSpriteState::OnMousePress(const ivec2& pos)
{
	m_first_pos.x = pos.x;
	m_first_pos.y = pos.y;
	m_pre_ori = m_ori;

	m_is_open = true;
}

void RotateSpriteState::OnMouseRelease(const ivec2& pos)
{
	m_is_open = false;
}

void RotateSpriteState::OnMouseMove(const ivec2& pos)
{
	if (!m_is_open) { return; }

	e3d::Camera& cam = m_canvas->GetCamera3();

	vec3 start = cam.MapToSphere(m_first_pos);
	vec3 end = cam.MapToSphere(ivec2(pos.x, pos.y));
	Quaternion delta = Quaternion::CreateFromVectors(start, end);
	m_ori = delta.Rotated(m_pre_ori);

  	e3d::ShaderMgr::Instance()->SetModelView(cam.GetMatrix());
	m_sprite->SetOri3(m_ori);

	m_canvas->Refresh();
}

}