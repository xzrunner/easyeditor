#include "Camera.h"

namespace e3d
{

const float Camera::CAM_NEAR = 4;
const float Camera::CAM_FAR = 10;

static const float DEFAULT_Z = -6;

	//const float Camera::CAM_NEAR = 1004;
	//const float Camera::CAM_FAR = 1010;

	//static const float DEFAULT_Z = -1008;

static const float ZOOM_STEP = 0.05f;

Camera::Camera()
	: m_pos(0, 0, DEFAULT_Z)
{
}

void Camera::Translate(const vec3& offset)
{
	m_pos += offset;
}

void Camera::Zoom(bool zoomin)
{
	if (zoomin) {
		m_pos.z *= (1 - ZOOM_STEP);
	} else {
		m_pos.z *= (1 + ZOOM_STEP);
	}
}

void Camera::SetRotate(const mat4& rot)
{
	m_rotation = rot;
}

mat4 Camera::GetMatrix() const
{
	return mat4::Translate(m_pos.x, m_pos.y, m_pos.z) * m_rotation;
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

void Camera::Reset()
{
	m_pos.x = m_pos.y = 0;
	m_pos.z = DEFAULT_Z;
	m_rotation = mat4::Identity();
}

}