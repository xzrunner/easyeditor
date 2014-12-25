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

}

#endif // _RT_CAMERA_H_