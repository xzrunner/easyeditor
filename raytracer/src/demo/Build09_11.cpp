#include "world/World.h"
#include "tracer/RayCast.h"
#include "lights/PointLight.h"
#include "cameras/Pinhole.h"
#include "objects/Plane.h"
#include "objects/Sphere.h"
#include "objects/Box.h"
#include "objects/Triangle.h"
#include "materials/Phong.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
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
	
// 	// for Figure 9.11(a)	
// 
// 	camera->SetEye(Point3D(0, 0, 500));
// 	camera->SetLookat(Point3D(0, 0, 0));    
// 	camera->SetViewDistance(500);
// 	camera->SetUpVector(Vector3D(1, 1, 0)); 

	
// 	// for Figure 9.11(b)
// 	
// 	camera->SetEye(Point3D(500, 0, 0));
// 	camera->SetLookat(Point3D(0, 0, 0));    
// 	camera->SetViewDistance(400);
// 	camera->SetUpVector(Vector3D(0, 1, -1));

	// for Figure 9.11(c)
		
	camera->SetEye(Point3D(300, 400, 500));
	camera->SetLookat(Point3D(-20, -30, -50)); 
	camera->SetViewDistance(400);
	camera->SetRollAngle(145);        // see Exercise 9.6

	
	camera->ComputeUVW();		

	m_camera = camera;
	

	PointLight* light = new PointLight;
	light->SetLocation(Point3D(50, 150, 200)); 
	light->ScaleRadiance(6.0);
	light->SetShadows(true);
	AddLight(light);
	
	
	// sphere
	
	Phong* phong1 = new Phong;			
	phong1->SetKa(0.5); 
	phong1->SetKd(0.4);
	phong1->SetCd(RGBColor(0.5, 0.6, 0));  	// green
	phong1->SetKs(0.05); 
	phong1->SetExp(25); 	
	
	Sphere*	sphere = new Sphere(Point3D(-45, 45, 40), 50); 
	sphere->SetMaterial(phong1);
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
	
	Phong*	phong2 = new Phong;			
	phong2->SetKa(0.25); 
	phong2->SetKd(0.5); 
	phong2->SetCd(RGBColor(0, 0.5, 0.5));     // cyan
	phong2->SetKs(0.05); 
	phong2->SetExp(50); 

	Triangle* triangle_ptr1 = new Triangle(Point3D(-110, -85, 80), Point3D(120, 10, 20), Point3D(-40, 50, -30));
	triangle_ptr1->SetMaterial(phong2);     
	AddObject(triangle_ptr1);
	
	
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