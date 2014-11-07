#include "RotateCameraState.h"
#include "StageCanvas.h"

namespace libsketch
{

static const float MOUSE_SENSITIVITY = 0.3f;

RotateCameraState::RotateCameraState(e3d::StageCanvas* canvas)
	: m_canvas(canvas)
{
}

void RotateCameraState::OnMousePress(const ivec2& pos)
{
	m_last_pos = pos;
}

void RotateCameraState::OnMouseRelease(const ivec2& pos)
{
}

void RotateCameraState::OnMouseMove(const ivec2& pos)
{
	e3d::Camera& cam = m_canvas->GetCamera3();
	float dx = (pos.x - m_last_pos.x) * MOUSE_SENSITIVITY,
		  dy = (m_last_pos.y - pos.y) * MOUSE_SENSITIVITY;
	cam.Rotate(dx, dy);
	m_canvas->Refresh();

	m_last_pos = pos;

	m_canvas->Refresh();
}

}