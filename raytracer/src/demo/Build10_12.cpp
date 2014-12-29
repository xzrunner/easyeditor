#include "world/World.h"
#include "tracer/RayCast.h"
#include "lights/Ambient.h"
#include "lights/Directional.h"
#include "samplers/MultiJittered.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
#include "materials/Reflective.h"
#include "cameras/ThinLens.h"
#include "objects/Box.h"
#include "objects/Plane.h"
#include "texture/Checker3D.h"
#include "utilities/ShadeRec.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 100;

	m_vp.SetWidth(450);                                                               	  		
	m_vp.SetHeight(300);
	m_vp.SetPixelSize(0.05);
	m_vp.SetSampler(new MultiJittered(num_samples)); 
	m_vp.SetMaxDepth(0);

	m_tracer = new RayCast(this);
	m_background_color = WHITE;

	Ambient* ambient = new Ambient;
	ambient->ScaleRadiance(0.5);
	m_ambient = ambient;

	ThinLens* thin_lens = new ThinLens;
	thin_lens->SetSampler(new MultiJittered(num_samples));
	thin_lens->SetEye(Point3D(15, 25, 50));  
	thin_lens->SetLookat(Point3D(0, 8, 0));
	thin_lens->SetViewDistance(30.0);
	thin_lens->SetFocalDistance(100);
	thin_lens->SetLensRadius(1);
	thin_lens->ComputeUVW();     
	m_camera = thin_lens;

	Directional* light = new Directional;
	light->SetDirection(Vector3D(1, 1, -0.5));     
	light->ScaleRadiance(4); 
	light->SetShadows(true);
	AddLight(light);

	Reflective* reflective_ptr = new Reflective;
	reflective_ptr->SetKr(0.9);
	reflective_ptr->SetCr(RGBColor(0.2, 0.9, 0.75));

	Box* box_ptr = new Box(Point3D(-17, 0, -1), Point3D(13, 19, 0));
	box_ptr->SetMaterial(reflective_ptr);
	AddObject(box_ptr);


	// ground plane

	Checker3D* checker_ptr = new Checker3D;
	checker_ptr->SetSize(8.0);
	checker_ptr->SetColor1(RGBColor(0.25, 0.25, 0.25));  			// gray
	checker_ptr->SetColor2(WHITE);	 

	SV_Matte* sv_matte_ptr = new SV_Matte;		
	sv_matte_ptr->SetKa(0.25);
	sv_matte_ptr->SetKd(0.75);
	sv_matte_ptr->SetCd(checker_ptr);	

	Plane* plane_ptr1 = new Plane(Point3D(0, 0, 0), Normal(0, 1, 0));
	plane_ptr1->SetMaterial(sv_matte_ptr);
	AddObject(plane_ptr1);

	// focal plane plane

	Matte* matte_ptr = new Matte;
	matte_ptr->SetCd(RGBColor(1, 0, 0));
	matte_ptr->SetKa(0.5);
	matte_ptr->SetKd(0.5);

	Point3D eye(15, 25, 50); 
	Point3D lookat(0, 8, 0); 
	Vector3D direction(lookat - eye); 
	direction.Normalize();
	float fd = 100;

	Point3D point = eye + direction * fd;
	Normal normal = -direction;

	Plane* plane_ptr2 = new Plane(point, normal); 
	plane_ptr2->SetMaterial(matte_ptr);
	AddObject(plane_ptr2);
}

}