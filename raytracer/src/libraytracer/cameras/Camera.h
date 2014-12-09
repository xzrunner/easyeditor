#ifndef _RT_CAMERA_H_
#define _RT_CAMERA_H_

#include "maths/Point3D.h"

namespace rt
{

class World;

class Camera
{
public:
	Camera();
	
	virtual void RenderScene(const World& w) const = 0;

	void SetEye(const Point3D& p);

	void SetLookat(const Point3D& p);

	void ComputeUVW();

protected:
	// eye point
	Point3D m_eye;
	// look at point
	Point3D m_lookat;

	// up vector
	Vector3D m_up;
	// orthonormal basis vectors
	Vector3D m_u, m_v, m_w;

	float m_exposure_time;

}; // Camera

inline Camera::
Camera()
	: m_eye(0, 0, 500)
	, m_lookat(0, 0, 0)
	, m_up(0, 1, 0)
	, m_u(1, 0, 0)
	, m_v(0, 1, 0)
	, m_w(0, 0, 1)
	, m_exposure_time(1)
{
}

inline void Camera::
SetEye(const Point3D& p)
{
	m_eye = p;
}

inline void Camera::
SetLookat(const Point3D& p)
{
	m_lookat = p;
}

}

#endif // _RT_CAMERA_H_