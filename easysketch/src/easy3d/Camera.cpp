#include "Camera.h"

namespace e3d
{

Camera::Camera()
{
	m_translation = mat4::Translate(0, 0, -1005);
}

void Camera::Translate(const vec3& offset)
{
	m_translation.Translate(offset.x, offset.y, offset.z);
}

void Camera::SetRotate(const mat4& rot)
{
	m_rotation = rot;
}

mat4 Camera::GetMatrix() const
{
	return m_rotation * m_translation;
}

void Camera::SetScreenSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

vec3 Camera::MapToSphere(ivec2 touchpoint) const
{
// 	int viewport[4];
// 	glGetIntegerv(GL_VIEWPORT, viewport);
// 	int width = viewport[2];
// 	int height = viewport[3];

	const float trackball_radius = m_width / 3;
	const vec2 center_point(m_width*0.5f, m_height*0.5f);

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