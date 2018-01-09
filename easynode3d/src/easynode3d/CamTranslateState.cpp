#include "CamTranslateState.h"
#include "StageCanvas.h"

namespace enode3d
{

CamTranslateState::CamTranslateState(StageCanvas& canvas, 
	                                       n3::Camera& cam,
	                                       const sm::vec2& pos)
	: m_canvas(canvas)
	, m_cam(cam)
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

	m_cam.Translate(dx * 0.01f, dy * 0.01f);

	m_last_pos = pos;

	m_canvas.Refresh();
}

}