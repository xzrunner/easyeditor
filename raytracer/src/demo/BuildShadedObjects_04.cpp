#include "World.h"

#include "samplers/Jittered.h"
#include "lights/Ambient.h"
#include "lights/Directional.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "texture/Checker3D.h"
#include "materials/SV_Matte.h"
#include "objects/Plane.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build()
{
	int num_samples = 1;  // use 1 for 4.8(a), 64 for 4.8(b)

	Jittered* sampler_ptr = new Jittered(num_samples);

	m_vp.SetWidth(400);
	m_vp.SetHeight(400);
	m_vp.SetSamples(num_samples);		// for 4.8(a) (one regular sample per pixel)
	//	m_vp.set_sampler(sampler_ptr);		// for 4.8(b)	

	m_background_color = RGBColor(0.25f, 0.25f, 0.25f);

	Ambient* ambient = new Ambient;
	ambient->ScaleRadiance(1.0);
	m_ambient = ambient;

	m_tracer = new RayCast(this);

	Pinhole* pinhole = new Pinhole;
	pinhole->SetEye(Point3D(100, 500, 100));
	pinhole->SetLookat(Point3D(0, 450, 0));
	pinhole->SetViewDistance(175);	
	pinhole->ComputeUVW();
	m_camera = pinhole;


	// directional light 

	Directional* light = new Directional;
	light->SetDirection(Point3D(0, 1, 0)); 
	light->ScaleRadiance(4.0);
	light->SetShadows(false);
	AddLight(light);


	// plane

	Checker3D* checker3d = new Checker3D;
	checker3d->SetSize(250.0);
	checker3d->SetColor1(BLACK);  	
	checker3d->SetColor2(WHITE); 

	SV_Matte* sv_matte = new SV_Matte;		
	sv_matte->SetKa(0.5);
	sv_matte->SetKd(0.35);
	sv_matte->SetCd(checker3d);	

	Plane* plane = new Plane(Point3D(0, 0, 0), Normal(0.0, 1.0, 0.0));
	plane->SetMaterial(sv_matte);
	AddObject(plane);	
}

}