#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "lights/PointLight.h"
#include "materials/Phong.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 16;
	
	m_vp.SetWidth(400);
	m_vp.SetHeight(400);
	m_vp.SetSamples(num_samples);
	
	m_tracer = new RayCast(this);

	Pinhole* pinhole = new Pinhole;	
		
//	pinhole->SetEye(Point3D(0, 0, 10));       	// for Figure 8.22(a)
//	pinhole->SetViewDistance(1687);
	
//	pinhole->SetEye(Point3D(0, 0, 5));       	// for Figure 8.22(b)
//	pinhole->SetViewDistance(750);
	
	pinhole->SetEye(Point3D(0, 0, 2.5));       	// for Figure 8.22(c)
	pinhole->SetViewDistance(280);
	
	pinhole->SetLookat(Point3D(0, 0, 0));
	pinhole->ComputeUVW(); 
	m_camera = pinhole;
	
	
	PointLight* light = new PointLight;
	light->SetLocation(Point3D(10, 50, 40));
	light->ScaleRadiance(3.0); 
	AddLight(light);
	
	Phong* phong = new Phong;			
	phong->SetKa(0.25); 
	phong->SetKd(0.65); 
	phong->SetCd(WHITE);   
	phong->SetKs(0.1); 
	phong->SetExp(25); 
	
	Point3D p0(-1.0, -1.0, -1.0);
	Point3D p1(1.0, 1.0, 1.0);
	float bevel_radius = 0.02;
	
	WireframeBox* box = new WireframeBox(p0, p1, bevel_radius);
	box->set_material(phong);
	AddObject(box);	
}

}