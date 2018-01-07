#include "TranslateCameraState.h"
#include "StageCanvas.h"

namespace e3d
{

TranslateCameraState::TranslateCameraState(StageCanvas& canvas, 
	                                       n3::Camera& cam,
	                                       const sm::vec2& pos)
	: m_canvas(canvas)
	, m_cam(cam)
	, m_last_pos(pos)
{
}

void TranslateCameraState::OnMousePress(const sm::vec2& pos)
{
	m_last_pos = pos;
}

void TranslateCameraState::OnMouseRelease(const sm::vec2& pos)
{
}

void TranslateCameraState::TranslateCameraState::OnMouseDrag(const sm::vec2& pos)
{
	float dx = pos.x - m_last_pos.x;
	float dy = pos.y - m_last_pos.y;

	m_cam.Translate(dx * 0.01f, dy * 0.01f);

	m_last_pos = pos;

	m_canvas.Refresh();
}

}