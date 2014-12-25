#include "Camera.h"

#include "maths/Matrix.h"
#include "utilities/Constants.h"

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

void Camera::SetUpVector(const Vector3D& up)
{
	m_up = m_up;
}

void Camera::SetRollAngle(float r)
{
	float ra = r * PI_ON_180;
	//T(1)*R(-1)R(a)R(1)T(-1)
	Matrix F,T,R,Ra;
	T.SetIdentity();
	R.SetIdentity();
	Ra.SetIdentity();

	Vector3D q = m_up;
	q.Normalize();
	Vector3D w = m_eye-m_lookat;
	w.Normalize();
	Vector3D p = w^q;
	p.Normalize();

	T.m[0][3] = m_lookat.x;
	T.m[1][3] = m_lookat.y;
	T.m[2][3] = m_lookat.z;

	R.m[0][0] = q.x;
	R.m[1][0] = q.y;
	R.m[2][0] = q.z;
	R.m[0][1] = p.x;
	R.m[1][1] = p.y;
	R.m[2][1] = p.z;
	R.m[0][2] = w.x;
	R.m[1][2] = w.y;
	R.m[2][2] = w.z;

	Ra.m[0][0] = cos(ra);
	Ra.m[0][1] = sin(ra);
	Ra.m[1][0] = -sin(ra);
	Ra.m[1][1] = cos(ra);

	F = (T*R)*Ra;

	T.m[0][3] = -m_lookat.x;
	T.m[1][3] = -m_lookat.y;
	T.m[2][3] = -m_lookat.z;

	R.m[0][0] = q.x;
	R.m[0][1] = q.y;
	R.m[0][2] = q.z;
	R.m[1][0] = p.x;
	R.m[1][1] = p.y;
	R.m[1][2] = p.z;
	R.m[2][0] = w.x;
	R.m[2][1] = w.y;
	R.m[2][2] = w.z;

	F = (F*R)*T;

	m_up = (F * (m_eye + m_up) - m_eye);
	m_up.Normalize();
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

	if (m_eye.x == m_lookat.x && m_eye.z == m_lookat.z && m_eye.y < m_lookat.y) { // camera looking vertically m_up
		m_u = Vector3D(1, 0, 0);
		m_v = Vector3D(0, 0, 1);
		m_w = Vector3D(0, -1, 0);
	}
}

}