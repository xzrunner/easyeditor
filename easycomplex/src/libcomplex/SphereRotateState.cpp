#include "SphereRotateState.h"

#include <ee/Vector.h>
#include <ee/EditPanelImpl.h>
#include <ee/StageCanvas.h>

namespace ecomplex
{

SphereRotateState::SphereRotateState(ee::EditPanelImpl* stage, const ee::Vector& first_pos,
									 sm::Quaternion& dir)
	: m_stage(stage)
	, m_dir(dir)
{
	ee::Vector p = m_stage->TransPosProjToScr(first_pos);
	m_last_pos.x = p.x;
	m_last_pos.y = p.y;
}

void SphereRotateState::OnMouseRelease(const ee::Vector& pos)
{
	m_last_pos.x = m_last_pos.y = static_cast<float>(INT_MAX);
}

bool SphereRotateState::OnMouseDrag(const ee::Vector& pos)
{
	if (m_last_pos.x == INT_MAX || m_last_pos.y == INT_MAX)	 {
		return false;
	}

	ee::Vector p = m_stage->TransPosProjToScr(pos);

	sm::vec3 start = MapToSphere(m_last_pos.x, m_last_pos.y),
		end = MapToSphere(p.x, p.y);
	sm::Quaternion delta = sm::Quaternion::CreateFromVectors(start, end);
	//	m_dir.Rotate(delta);
	m_dir = delta.Rotated(m_dir);

	m_last_pos.x = p.x;
	m_last_pos.y = p.y;

	return true;
}

sm::vec3 SphereRotateState::MapToSphere(int x, int y)
{
	int w = m_stage->GetCanvas()->GetWidth(),
		h = m_stage->GetCanvas()->GetHeight();
	const float trackball_radius = w / 3;
	const sm::vec2 center_point(w*0.5f, h*0.5f);

	sm::vec2 p = sm::vec2(x, y) - center_point;

	// Flip the Y axis because pixel coords increase towards the bottom.
	p.y = -p.y;

	const float radius = trackball_radius;
	const float safeRadius = radius - 1;

	if (p.Length() > safeRadius) {
		float theta = atan2(p.y, p.x);
		p.x = safeRadius * cos(theta);
		p.y = safeRadius * sin(theta);
	}

	float z = sqrt(radius * radius - p.LengthSquared());
	sm::vec3 mapped = sm::vec3(p.x, p.y, z);
	return mapped / radius;
}

}