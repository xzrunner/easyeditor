#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "lights/PointLight.h"
#include "materials/Phong.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
#include "objects/Sphere.h"
#include "objects/Box.h"
#include "objects/Plane.h"
#include "objects/Triangle.h"
#include "texture/Checker3D.h"
#include "utilities/RGBColor.h"

namespace rt
{

void World::Build()
{
	int num_samples = 1;
	
	m_vp.SetWidth(300);			
	m_vp.SetHeight(300);
	m_vp.SetSamples(num_samples);
	
	m_tracer = new RayCast(this);
	Pinhole* camera = new Pinhole;
	
// 	// for Figure 9.9(a)	
// 
// 	camera->SetEye(Point3D(0, 0, 500));
// 	camera->SetLookat(Point3D(0, 0, 0));    
// 	camera->SetViewDistance(500); 
		
 	// for Figure 9.9(b)
 	
 	camera->SetEye(Point3D(300, 400, 500));
 	camera->SetLookat(Point3D(0, 0, -50));
 	camera->SetViewDistance(400);

// 	// for Figure 9.9(c)
// 	
// 	camera->SetEye(Point3D(-1000, 2000, -500));
// 	camera->SetLookat(Point3D(0, -100, 0));
// 	camera->SetViewDistance(250);
	
	camera->ComputeUVW();		  

	m_camera = camera;
	

	PointLight* light = new PointLight;
	light->SetLocation(Point3D(50, 150, 200)); 
	light->ScaleRadiance(6.0);
	light->SetShadows(true);
	AddLight(light);
	
	
	// sphere
	
	Phong* phong = new Phong;			
	phong->SetKa(0.5); 
	phong->SetKd(0.4);
	phong->SetCd(RGBColor(0.5, 0.6, 0));  	// green
	phong->SetKs(0.05); 
	phong->SetExp(25); 	
	
	Sphere*	sphere = new Sphere(Point3D(-45, 45, 40), 50); 
	sphere->SetMaterial(phong);
	AddObject(sphere);
	
	
	// box
	
	Matte* matte = new Matte;				
	matte->SetKa(0.4); 
	matte->SetKd(0.3);
	matte->SetCd(RGBColor(0.8, 0.5, 0));  	// orange

	Box* box = new Box(Point3D(20, -101, -100), Point3D(90, 100, 20));
	box->SetMaterial(matte);
	AddObject(box);

	
	// triangle
	
	Phong*	phong_ptr2 = new Phong;			
	phong_ptr2->SetKa(0.25); 
	phong_ptr2->SetKd(0.5); 
	phong_ptr2->SetCd(RGBColor(0, 0.5, 0.5));     // cyan
	phong_ptr2->SetKs(0.05); 
	phong_ptr2->SetExp(50); 

	Triangle* triangle = new Triangle(Point3D(-110, -85, 80), Point3D(120, 10, 20), Point3D(-40, 50, -30));
	triangle->SetMaterial(phong_ptr2);     
	AddObject(triangle);
	
	
	// ground plane with checker
	
	Checker3D* checker3D = new Checker3D;
	checker3D->SetSize(100); 
	checker3D->SetColor1(RGBColor(0.7, 0.7, 0.7));  
	checker3D->SetColor2(RGBColor(1, 1, 1));

	SV_Matte* sv_matte = new SV_Matte;
	sv_matte->SetKa(0.25);
	sv_matte->SetKd(0.35);
	sv_matte->SetCd(checker3D);
	
	Plane* plane = new Plane(Point3D(0, -101, 0), Normal(0, 1, 0));
	plane->SetMaterial(sv_matte);
	AddObject(plane);	
}

}