#include "FishEye.h"

#include "world/ViewPlane.h"
#include "world/World.h"
#include "tracer/Tracer.h"
#include "samplers/Sampler.h"
#include "utilities/RGBColor.h"
#include "utilities/Constants.h"

namespace rt
{

FishEye::FishEye()
	: m_psi_max(180.0)
{
}

void FishEye::RenderScene(const World& wr) const
{
	RGBColor	L;
	const ViewPlane& vp = wr.GetViewPlane();
	int 		hres		= vp.GetWidth();
	int 		vres 		= vp.GetHeight();
	float		s 			= vp.GetPixelSize();
	Ray			ray;
	int 		depth 		= 0;
	Point2D 	sp; 					// sample point in [0, 1] X [0, 1]
	Point2D 	pp;						// sample point on the pixel
	float		r_squared;				// sum of squares of normalised device coordinates

	ray.ori = m_eye;

	for (int r = 0; r < vres; r++) {		// up
		for (int c = 0; c < hres; c++) {	// across
			L = BLACK;
			for (int j = 0; j < vp.GetSamplesNum(); j++) {
				sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = s * (c - 0.5 * hres + sp.x);
				pp.y = s * (r - 0.5 * vres + sp.y);
				ray.dir = RayDirection(pp, hres, vres, s, r_squared);

				if (r_squared <= 1.0)
					L += wr.GetTracer()->TraceRay(ray, depth);
			}

			L /= vp.GetSamplesNum();
			L *= m_exposure_time;

			wr.DisplayPixel(r, c, L);
		}	
	}
}

void FishEye::SetFov(float fov)
{
	m_psi_max = fov;
}

Vector3D FishEye::RayDirection(const Point2D& pp, const int hres, const int vres, 
							   const float s, float& r_squared) const
{
	// compute the normalized device coordinates
	Point2D pn(2.0 / (s * hres) * pp.x, 2.0 / (s * vres) * pp.y);
	r_squared = pn.x * pn.x + pn.y * pn.y;

	if (r_squared <= 1.0) {
		float r 		= sqrt(r_squared);
		float psi 		= r * m_psi_max * PI_ON_180;
		float sin_psi 	= sin(psi);
		float cos_psi 	= cos(psi);
		float sin_alpha = pn.y / r;
		float cos_alpha = pn.x / r;
		Vector3D dir 	= sin_psi * cos_alpha * m_u +  sin_psi * sin_alpha * m_v - cos_psi * m_w;

		return dir;
	} else {
		return Vector3D(0, 0, 0);
	}
}

}