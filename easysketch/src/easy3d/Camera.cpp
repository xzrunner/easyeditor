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

}