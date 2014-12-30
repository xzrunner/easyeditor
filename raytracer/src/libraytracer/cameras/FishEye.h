#ifndef _RT_FISH_EYE_H_
#define _RT_FISH_EYE_H_

#include "Camera.h"

#include "maths/Point2D.h"

namespace rt
{

class FishEye : public Camera
{
public:
	FishEye();

	virtual void RenderScene(const World& w) const;

	void SetFov(float fov);

private:
	Vector3D RayDirection(const Point2D& pixel_point, const int hres,
		const int vres, const float s, float& r_squared) const;

private:
	// in degrees
	float m_psi_max;

}; // FishEye

}

#endif // _RT_FISH_EYE_H_