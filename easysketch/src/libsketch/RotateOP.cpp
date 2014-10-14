#include "RotateOP.h"

namespace libsketch
{

RotateOP::RotateOP(d2d::EditPanel* stage)
	: d2d::AbstractEditOP(stage)
{
}

bool RotateOP::onMouseLeftDown(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseLeftDown(x, y)) { return true; }

	m_first_pos.x = x;
	m_first_pos.y = y;
	m_pre_ori = m_ori;

	return false;
}

bool RotateOP::onMouseDrag(int x, int y)
{
	if (d2d::AbstractEditOP::onMouseDrag(x, y)) { return true; }
	
	vec3 start = MapToSphere(m_first_pos);
	vec3 end = MapToSphere(ivec2(x, y));
	Quaternion delta = Quaternion::CreateFromVectors(start, end);
	m_ori = delta.Rotated(m_pre_ori);

	d2d::ShaderNew::Instance()->SetModelView(m_ori);
	
	m_editPanel->Refresh();

	return false;
}

vec3 RotateOP::MapToSphere(ivec2 touchpoint) const
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width = viewport[2];
	int height = viewport[3];

	const float trackball_radius = width / 3;
	const vec2 center_point(width*0.5f, height*0.5f);

	vec2 p = (vec2)touchpoint - center_point;

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