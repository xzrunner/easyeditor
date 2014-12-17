#include "world/World.h"
#include "samplers/MultiJittered.h"
#include "tracer/AreaLighting.h"
#include "cameras/Pinhole.h"
#include "materials/Emissive.h"
#include "materials/Reflective.h"
#include "materials/SV_Matte.h"
#include "objects/Rectangle.h"
#include "objects/Sphere.h"
#include "objects/Plane.h"
#include "texture/Checker3D.h"
#include "lights/AreaLight.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 16;
	Sampler* sampler = new MultiJittered(num_samples);

	m_vp.SetWidth(400);
	m_vp.SetHeight(400);
	m_vp.SetMaxDepth(1);
	m_vp.SetSampler(sampler);	

	m_background_color = BLACK;

	m_tracer = new AreaLighting(this);

	Pinhole* pinhole = new Pinhole;
	pinhole->SetEye(Point3D(30, 13, 20));
	pinhole->SetLookat(Point3D(0, -2, 0)); 
	pinhole->SetViewDistance(12000); 
	pinhole->ComputeUVW();
	m_camera = pinhole; 

	Emissive* emissive = new Emissive;
	emissive->SetRadianceScaleFactor(3.0);
	emissive->SetColor(WHITE);

	Rectangle* rectangle = new Rectangle(Point3D(-1, -0.5, -1), Vector3D(2, 0, 0), Vector3D(0, 0, 2), Normal(0, -1, 0));
	rectangle->SetMaterial(emissive);
	rectangle->SetSampler(sampler);
	AddObject(rectangle);

	AreaLight* rect_light = new AreaLight;
	rect_light->SetObject(rectangle);
	rect_light->SetShadows(true);
	AddLight(rect_light);


	Reflective* reflective = new Reflective;
	reflective->SetKa(0.2); 
	reflective->SetKd(0.1); 
	reflective->SetCd(RGBColor(0, 1, 0.2));  // green
	reflective->SetKs(0.0);     
	reflective->SetExp(1);
// 	reflective_ptr1->set_kr(0.85); 
// 	reflective_ptr1->set_cr(0.75, 0.75, 1);  // blue 

	Sphere* sphere = new Sphere(Point3D(0, -2, 0), 0.5); 	
	sphere->SetMaterial(reflective);
	AddObject(sphere);


	Checker3D* checker = new Checker3D;
	checker->SetSize(1);		
	checker->SetColor1(RGBColor(1, 1, 1));  
	checker->SetColor2(RGBColor(0.9, 0.9, 0.9));

	SV_Matte* sv_matte = new SV_Matte;		
	sv_matte->SetKa(0.25);
	sv_matte->SetKd(0.75);
	sv_matte->SetCd(checker);

	Plane* plane = new Plane(Point3D(0, -2.75, 0), Normal(0, 1, 0));  
	plane->SetMaterial(sv_matte);
	AddObject(plane);	
}

}