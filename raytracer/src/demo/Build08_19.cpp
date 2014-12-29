#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "lights/PointLight.h"
#include "materials/Phong.h"
#include "materials/Matte.h"
#include "objects/Triangle.h" 
#include "objects/Sphere.h"
#include "objects/Box.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 1;

	m_vp.SetWidth(400);			
	m_vp.SetHeight(400);
	m_vp.SetSamples(num_samples);

	m_background_color = BLACK;
	m_tracer = new RayCast(this);

	Pinhole* camera = new Pinhole;
	// Figure 8.19
	camera->SetEye(Point3D(0, 0, 200));     // for Figure 8.19(a)
	//camera->SetEye(Point3D(0, 0, 135));     // for Figure 8.19(b)
	//camera->SetEye(Point3D(0, 0, 75));      // for Figure 8.19(c)
	camera->SetViewDistance(100);

// 	// Figure 8.16
// 	camera->SetEye(Point3D(0, 0, 250));
// 	//camera->SetViewDistance(200);  	// for Figure 8.16(a)	
// 	//camera->SetViewDistance(450);  	// for Figure 8.16(b)		
// 	camera->SetViewDistance(1000);  	// for Figure 8.16(c)

	camera->ComputeUVW();		  
	m_camera = camera;


	PointLight* light = new PointLight;
	light->SetLocation(Point3D(50, 150, 200)); 
	light->ScaleRadiance(6.0);
	light->SetShadows(true);
	AddLight(light);


	// sphere

	Phong* phong = new Phong;			
	phong->SetKa(0.5f); 
	phong->SetKd(0.4f);
	phong->SetCd(RGBColor(0.5f, 0.6f, 0));  	// green
	phong->SetKs(0.05f); 
	phong->SetExp(25); 	

	Sphere*	sphere = new Sphere(Point3D(-45, 45, 40), 50); 
	sphere->SetMaterial(phong);
	AddObject(sphere);


	// box

	Matte* matte = new Matte;				
	matte->SetKa(0.4f); 
	matte->SetKd(0.3f);
	matte->SetCd(RGBColor(0.8f, 0.5f, 0));  	// orange

	Box* box = new Box(Point3D(20, -101, -100), Point3D(90, 100, 20));
	box->SetMaterial(matte);
	AddObject(box);


	// triangle

	Phong*	phong_ptr2 = new Phong;			
	phong_ptr2->SetKa(0.25f); 
	phong_ptr2->SetKd(0.5f); 
	phong_ptr2->SetCd(RGBColor(0, 0.5f, 0.5f));     // cyan
	phong_ptr2->SetKs(0.05f); 
	phong_ptr2->SetExp(50); 

	Triangle* triangle = new Triangle(Point3D(-110, -85, 80), Point3D(120, 10, 20), Point3D(-40, 50, -30));
	triangle->SetMaterial(phong_ptr2);     
	AddObject(triangle);
}

}