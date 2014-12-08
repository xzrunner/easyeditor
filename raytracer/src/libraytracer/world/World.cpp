#include "World.h"

#include "objects/GeometricObject.h"
#include "lights/Light.h"
#include "lights/Ambient.h"
#include "samplers/Sampler.h"
#include "tracer/Tracer.h"
#include "maths/Point2D.h"
#include "maths/Point3D.h"
#include "maths/Ray.h"
#include "utilities/utility.h"
#include "utilities/RGBColor.h"
#include "utilities/Constants.h"

#include <algorithm>
#include <float.h>

namespace rt
{

World::World()
	: m_ambient(new Ambient)
	, m_tracer(NULL)
	, m_background_color(BLACK)
{
}

World::~World()
{
	for_each(m_objects.begin(), m_objects.end(), DeletePointerFunctor<GeometricObject>());
	for_each(m_lights.begin(), m_lights.end(), DeletePointerFunctor<Light>());

	delete m_ambient;
	delete m_tracer;
}

void World::AddObject(GeometricObject* obj)
{
	m_objects.push_back(obj);
}

void World::AddLight(Light* light)
{
	m_lights.push_back(light);
}

void World::RenderScene()
{
	RGBColor pixel_color;
	Ray	ray;
	Point2D pos;

	int width = m_vp.GetWidth(),
		height = m_vp.GetHeight();
	float size = m_vp.GetPixelSize();

	// hardwired in
	float zw = 100.0;

	for (int y = 0, h = m_vp.GetHeight(); y < h; ++y) {
		for (int x = 0, w = m_vp.GetWidth(); x < w; ++x) {
			for (int i = 0, n = m_vp.GetSamplesNum(); i < n; ++i) {
				const Point2D& sp = m_vp.GetSampler()->SampleUnitSquare();
				pos.x = size * (x - 0.5f * width + sp.x);
				pos.y = size * (y - 0.5f * height + sp.y);
				ray.ori = Point3D(size * (x - width / 2.0f + 0.5f), size * (y - height / 2.0f + 0.5f), zw);
				pixel_color += m_tracer->TraceRay(ray);
			}
		}
	}
}

ShadeRec World::HitObjects(const Ray& ray) const
{
	ShadeRec	sr(*this);
	double		t;
	Normal		normal;
	Point3D		local_hit_point;
	double		tmin = DBL_MAX;
	for (int i = 0, n = m_objects.size(); i < n; i++) 
	{
		if (m_objects[i]->Hit(ray, t, sr) && (t < tmin)) {
			sr.hit_an_object	= true;
			tmin 				= t;
			sr.material			= m_objects[i]->GetMaterial();
			sr.hit_point 		= ray.ori + t * ray.dir;
			normal 				= sr.normal;
			local_hit_point	 	= sr.local_hit_point;
		}
	}

	if(sr.hit_an_object) {
		sr.t = tmin;
		sr.normal = normal;
		sr.local_hit_point = local_hit_point;
	}

	return(sr);
}

}