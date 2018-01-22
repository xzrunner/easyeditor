#include "CamTranslateState.h"
#include "StageCanvas.h"

#include <ee/panel_msg.h>

namespace enode3d
{

CamTranslateState::CamTranslateState(n3::Camera& cam, const sm::vec2& pos)
	: m_cam(cam)
	, m_last_pos(pos)
{
}

void CamTranslateState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void CamTranslateState::OnMouseRelease(const sm::vec2& pos)
{
}

void CamTranslateState::CamTranslateState::OnMouseDrag(const sm::vec2& pos)
{
	float dx = pos.x - m_last_pos.x;
	float dy = pos.y - m_last_pos.y;

	float dist = m_cam.GetDistance();
	static const float SCALE = 0.005f;
	m_cam.Translate(dx * dist * SCALE, dy * dist * SCALE);

	m_last_pos = pos;

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
}

}