#include "ThinLens.h"

#include "maths/Ray.h"
#include "world/ViewPlane.h"
#include "world/World.h"
#include "samplers/Sampler.h"
#include "tracer/Tracer.h"
#include "utilities/RGBColor.h"
#include "utilities/Constants.h"

namespace rt
{

ThinLens::ThinLens()
	: m_lens_radius(1)
	, m_d(500)
	, m_f(75)
	, m_zoom(1)
	, m_sampler(NULL)
{
}

ThinLens::~ThinLens()
{
	if (m_sampler) {
		m_sampler->Release();
	}
}

void ThinLens::RenderScene(const World& world) const
{
	RGBColor	L;
	Ray			ray;
	ViewPlane	vp(world.GetViewPlane());
	int 		depth 		= 0;

	Point2D sp;			// sample point in [0, 1] X [0, 1]
	Point2D pp;			// sample point on a pixel
	Point2D dp;			// sample point on unit disk
	Point2D lp;			// sample point on lens

	vp.SetPixelSize(vp.GetPixelSize() / m_zoom);

	int w = vp.GetWidth(),
		h = vp.GetHeight();
	float s = vp.GetPixelSize();
	for (int r = 0; r < h; r++)	{		// up
		for (int c = 0; c < w; c++) {		// across
			L = BLACK;
			for (int n = 0; n < vp.GetSamplesNum(); n++) {
				sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = s * (c - w / 2.0f + sp.x);
				pp.y = s * (r - h / 2.0f + sp.y);

				dp = m_sampler->SampleUnitDisk();
				lp = dp * m_lens_radius;

				ray.ori = m_eye + lp.x * m_u + lp.y * m_v;
				ray.dir = RayDirection(pp, lp);
				L += world.GetTracer()->TraceRay(ray, depth);
			}

			L /= vp.GetSamplesNum();
			L *= m_exposure_time;
			world.DisplayPixel(r, c, L);
		}
	}
}

// ----------------------------------------------------------------------------- ray direction
// explained on page 173
Vector3D ThinLens::RayDirection(const Point2D& pixel_point, const Point2D& lens_point) const
{
	// hit point on focal plane
	Point2D p(pixel_point.x * m_f / m_d, pixel_point.y * m_f / m_d);
	Vector3D dir = (p.x - lens_point.x) * m_u + (p.y - lens_point.y) * m_v - m_f * m_w;
	dir.Normalize();
	return dir;
}

void ThinLens::SetSampler(Sampler* sampler)
{
	obj_assign((const Object*&)m_sampler, sampler);
	m_sampler->MapSamplesToUnitDisk();
}

}