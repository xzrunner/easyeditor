#ifndef _RT_PINHOLE_H_
#define _RT_PINHOLE_H_

#include "Camera.h"

#include "maths/Vector3D.h"

namespace rt
{

class Point2D;

class Pinhole : public Camera
{
public:
	Pinhole();
	
	virtual void RenderScene(const World& w) const;

	void SetViewDistance(const float vpd);

	Vector3D GetDirection(const Point2D& p) const;

private:
	// view plane distance
	float m_d;
	// zoom factor
	float m_zoom;

}; // Pinhole

inline void Pinhole::
SetViewDistance(const float vpd)
{
	m_d = vpd;
}

}

#endif // _RT_PINHOLE_H_