#include "painting3/Viewport.h"
#include "painting3/Camera.h"

namespace pt3
{

Viewport::Viewport()
	: m_width(0)
	, m_height(0)
{
}

void Viewport::SetSize(float width, float height)
{
	m_width  = width;
	m_height = height;
}

sm::vec2 Viewport::TransPos3ProjectToScreen(const sm::vec3& proj, const Camera& cam) const
{
	float fovy = cam.GetAngleOfView();
	float aspect = cam.GetAspect();
	float tan_half_fovy = tan(fovy / 2 * SM_DEG_TO_RAD);

	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;

	float x = proj.x / proj.z;
	float y = proj.y / proj.z;

	float sx = x / (tan_half_fovy * aspect) * hw + hw;
	float sy = - y / tan_half_fovy * hh + hh;

	return sm::vec2(sx, sy);
}

sm::vec3 Viewport::TransPos3ScreenToDir(const sm::vec2& screen, const Camera& cam) const
{
	float fovy = cam.GetAngleOfView();
	float aspect = cam.GetAspect();
	float tan_half_fovy = tan(fovy / 2 * SM_DEG_TO_RAD);

	float hw = m_width * 0.5f;
	float hh = m_height * 0.5f;

	float x = (screen.x - hw) / hw * tan_half_fovy * aspect;
	float y = - (screen.y - hh) / hh * tan_half_fovy;
	float z = 1;

	return cam.GetRotateMat().Inverted() * sm::vec3(x, y, z);
}

sm::vec3 Viewport::MapToSphere(const sm::vec2& touchpoint) const
{
	const float trackball_radius = m_width / 3.0f;
	const sm::vec2 center_point(m_width*0.5f, m_height*0.5f);

	sm::vec2 p = touchpoint - center_point;

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