#include "world/World.h"
#include "samplers/MultiJittered.h"
#include "tracer/AreaLighting.h"
#include "cameras/Pinhole.h"
#include "materials/Emissive.h"
#include "objects/Rectangle.h"
#include "lights/AreaLight.h"
#include "utilities/Constants.h"

namespace rt
{

void World::build(void) 
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
//	rectangle->set_material(emissive);
	rectangle->SetSampler(sampler);
	AddObject(rectangle);

	AreaLight* rect_light = new AreaLight;
	rect_light->SetObject(rectangle);
	rect_light->SetShadows(true);
	AddLight(rect_light);


	Reflective* reflective_ptr1 = new Reflective;
	reflective_ptr1->set_ka(0.2); 
	reflective_ptr1->set_kd(0.1); 
	reflective_ptr1->set_cd(0, 1, 0.2);  // green
	reflective_ptr1->set_ks(0.0);     
	reflective_ptr1->set_exp(1);
	reflective_ptr1->set_kr(0.85); 
	reflective_ptr1->set_cr(0.75, 0.75, 1);  // blue 

	Sphere* sphere_ptr1 = new Sphere(Point3D(0, -2, 0), 0.5); 	
	sphere_ptr1->set_material(reflective_ptr1);
	add_object(sphere_ptr1);


	Checker3D* checker_ptr = new Checker3D;
	checker_ptr->set_size(1);		
	checker_ptr->set_color1(1.0);  
	checker_ptr->set_color2(0.9);

	SV_Matte* sv_matte_ptr = new SV_Matte;		
	sv_matte_ptr->set_ka(0.25);
	sv_matte_ptr->set_kd(0.75);
	sv_matte_ptr->set_cd(checker_ptr);

	Plane* plane_ptr = new Plane(Point3D(0, -2.75, 0), Normal(0, 1, 0));  
	plane_ptr->set_material(sv_matte_ptr);
	add_object(plane_ptr);	
}

}