#include "Pinhole.h"

#include "utilities/RGBColor.h"
#include "utilities/Constants.h"
#include "maths/Point2D.h"
#include "world/World.h"
#include "samplers/Sampler.h"
#include "tracer/Tracer.h"

namespace rt
{

Pinhole::Pinhole()
	: m_d(500)
	, m_zoom(1.0)
{
}

void Pinhole::RenderScene(const World& world) const
{
	RGBColor	L;
	const ViewPlane& vp = world.GetViewPlane();
	Ray			ray;
	int 		depth = 0;	// recursion depth
	Point2D 	sp;			// sample point in [0, 1] x [0, 1]
	Point2D 	pp;			// sample point on a pixel
	int n = (int)sqrt((float)vp.GetSamplesNum());

	float sz = vp.GetPixelSize() / m_zoom;
	ray.ori = m_eye;

	int w = vp.GetWidth(),
		h = vp.GetHeight();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			L = BLACK;
			for (int j = 0; j < vp.GetSamplesNum(); j++) {
				sp = vp.GetSampler()->SampleUnitSquare();
				pp.x = sz * (x - 0.5f * h + sp.x);
				pp.y = sz * (y - 0.5f * w + sp.y);
				ray.dir = GetDirection(pp);
				L += world.GetTracer()->TraceRay(ray, depth);
			}

			L /= vp.GetSamplesNum();
			L *= m_exposure_time;

			world.DisplayPixel(y, x, MaxToOneColor(L));
		}
	}
}

Vector3D Pinhole::GetDirection(const Point2D& p) const
{
	Vector3D dir = p.x * m_u + p.y * m_v - m_d * m_w;
	dir.Normalize();
	return dir;
}

}