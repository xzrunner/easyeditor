#include "TranslateCameraState.h"
#include "StageCanvas.h"

static const float CAM_SPEED = 1 / 500.0f;

namespace esketch
{

TranslateCameraState::TranslateCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
	, m_is_open(false)
{
}

void TranslateCameraState::OnMousePress(const ee::ivec2& pos)
{
	m_is_open = true;
	m_last_pos = pos;
}

void TranslateCameraState::OnMouseRelease(const ee::ivec2& pos)
{
	m_is_open = false;	
}

void TranslateCameraState::OnMouseMove(const ee::ivec2& pos)
{
	if (!m_is_open) { return; }

	ee::vec3 offset;
	offset.x = (pos.x - m_last_pos.x) * CAM_SPEED;
	offset.y = -(pos.y - m_last_pos.y) * CAM_SPEED;

	e3d::Camera& cam = m_canvas->GetCamera3();
	cam.Translate(-offset);

	m_last_pos = pos;

	e3d::ShaderMgr::Instance()->SetModelView(cam.GetModelViewMat());

	m_canvas->Refresh();
}

}