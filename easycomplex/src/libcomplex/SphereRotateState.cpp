#include "SphereRotateState.h"

namespace ecomplex
{

SphereRotateState::SphereRotateState(d2d::EditPanelImpl* stage, const d2d::Vector& first_pos,
									 Quaternion& dir)
	: m_stage(stage)
	, m_dir(dir)
{
	d2d::Vector p = m_stage->TransPosProjToScr(first_pos);
	m_last_pos.x = p.x;
	m_last_pos.y = p.y;
}

d2d::AbstractAtomicOP* SphereRotateState::OnMouseRelease(const d2d::Vector& pos)
{
	m_last_pos.x = m_last_pos.y = INT_MAX;
	return NULL;
}

bool SphereRotateState::OnMouseDrag(const d2d::Vector& pos)
{
	if (m_last_pos.x == INT_MAX || m_last_pos.y == INT_MAX)	 {
		return false;
	}

	d2d::Vector p = m_stage->TransPosProjToScr(pos);

	vec3 start = MapToSphere(m_last_pos.x, m_last_pos.y),
		end = MapToSphere(p.x, p.y);
	Quaternion delta = Quaternion::CreateFromVectors(start, end);
	//	m_dir.Rotate(delta);
	m_dir = delta.Rotated(m_dir);

	m_last_pos.x = p.x;
	m_last_pos.y = p.y;

	return true;
}

vec3 SphereRotateState::MapToSphere(int x, int y)
{
	int w = m_stage->GetCanvas()->GetWidth(),
		h = m_stage->GetCanvas()->GetHeight();
	const float trackball_radius = w / 3;
	const vec2 center_point(w*0.5f, h*0.5f);

	vec2 p = vec2(x, y) - center_point;

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
	vec3 mapped = vec3(p.x, p.y, z);
	return mapped / radius;
}

}