#ifndef _RT_THINLENS_H_
#define _RT_THINLENS_H_

#include "Camera.h"

#include "maths/Point2D.h"

namespace rt
{

class Sampler;

class ThinLens : public Camera
{
public:
	ThinLens();
	virtual ~ThinLens();

	virtual void RenderScene(const World& w) const;

	void SetLensRadius(float radius) { m_lens_radius = radius; }
	void SetViewDistance(const float vpd) { m_d = vpd; }
	void SetFocalDistance(const float f) { m_f = f; }
	void SetZoom(const float zoom_factor) { m_zoom = zoom_factor; }
	void SetSampler(Sampler* sampler);

private:
	Vector3D RayDirection(const Point2D& pixel_point, const Point2D& lens_point) const;

private:
	float		m_lens_radius;	// lens radius
	float		m_d;			// view plane distance
	float		m_f;			// focal distance
	float		m_zoom;			// zoom factor
	Sampler*	m_sampler;		// sampler object

}; // ThinLens

}

#endif // _RT_THINLENS_H_