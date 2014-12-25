#include "Camera.h"

namespace rt
{

Camera::Camera()
	: m_eye(0, 0, 500)
	, m_lookat(0, 0, 0)
	, m_up(0, 1, 0)
	, m_u(1, 0, 0)
	, m_v(0, 1, 0)
	, m_w(0, 0, 1)
	, m_exposure_time(1)
{
}

void Camera::SetEye(const Point3D& p)
{
	m_eye = p;
}

void Camera::SetLookat(const Point3D& p)
{
	m_lookat = p;
}

void Camera::ComputeUVW()
{
	m_w = m_eye - m_lookat;
	m_w.Normalize();
	m_u = m_up ^ m_w;
	m_u.Normalize();
	m_v = m_w ^ m_u;

	// take care of the singularity by hardwiring in specific camera orientations

	if (m_eye.x == m_lookat.x && m_eye.z == m_lookat.z && m_eye.y > m_lookat.y) { // camera looking vertically down
		m_u = Vector3D(0, 0, 1);
		m_v = Vector3D(1, 0, 0);
		m_w = Vector3D(0, 1, 0);
	}

	if (m_eye.x == m_lookat.x && m_eye.z == m_lookat.z && m_eye.y < m_lookat.y) { // camera looking vertically up
		m_u = Vector3D(1, 0, 0);
		m_v = Vector3D(0, 0, 1);
		m_w = Vector3D(0, -1, 0);
	}
}

}