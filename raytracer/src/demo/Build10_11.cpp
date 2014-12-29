#include "world/World.h"
#include "tracer/RayCast.h"
#include "lights/Ambient.h"
#include "lights/Directional.h"
#include "samplers/MultiJittered.h"
#include "materials/SV_Matte.h"
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

	m_vp.SetWidth(400);                                                               	  		
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
	thin_lens->SetEye(Point3D(0, 6, 50));  
	thin_lens->SetLookat(Point3D(0, 6, 0));
	thin_lens->SetViewDistance(40.0);

// 	thin_lens->SetFocalDistance(98.0);	// for Figure 10.11(a)
// 	thin_lens->SetLensRadius(3.0);		// for Figure 10.11(a)

	thin_lens->SetFocalDistance(100000);	// for Figure 10.11(b)
	thin_lens->SetLensRadius(0.25);		// for Figure 10.11(]b)

	thin_lens->ComputeUVW();     
	m_camera = thin_lens;

	Directional* light = new Directional;
	light->SetDirection(Vector3D(1, 1, 1));     
	light->ScaleRadiance(7.5); 
	light->SetShadows(true);
	AddLight(light);


	// box 1

	Checker3D* checker_ptr1 = new Checker3D;
	checker_ptr1->SetSize(2.0);
	checker_ptr1->SetColor1(RGBColor(1, 1, 0.33));  		// lemon
	checker_ptr1->SetColor2(BLACK);	 

	SV_Matte* sv_matte_ptr1 = new SV_Matte;		
	sv_matte_ptr1->SetKa(0.5);
	sv_matte_ptr1->SetKd(0.35);
	sv_matte_ptr1->SetCd(checker_ptr1);

	Box* box_ptr1 = new Box(Point3D(-9, 0, -1), Point3D(-3, 12, 0));
	box_ptr1->SetMaterial(sv_matte_ptr1);
	AddObject(box_ptr1);


	// box 2

	Checker3D* checker_ptr2 = new Checker3D;
	checker_ptr2->SetSize(2.0);
	checker_ptr2->SetColor1(BLACK);  	
	checker_ptr2->SetColor2(RGBColor(0.1, 1, 0.5));	  	// green

	SV_Matte* sv_matte_ptr2 = new SV_Matte;		
	sv_matte_ptr2->SetKa(0.5);
	sv_matte_ptr2->SetKd(0.35);
	sv_matte_ptr2->SetCd(checker_ptr2);	

	Box* box_ptr2 = new Box(Point3D(-3.25, 0, -25), Point3D(4.75, 14, -24));
	box_ptr2->SetMaterial(sv_matte_ptr2);
	AddObject(box_ptr2);


	// box 3

	Checker3D* checker_ptr3 = new Checker3D;
	checker_ptr3->SetSize(2.0);
	checker_ptr3->SetColor1(BLACK);  	
	checker_ptr3->SetColor2(RGBColor(1, 0.6, 0.15));	  	// orange

	SV_Matte* sv_matte_ptr3 = new SV_Matte;		
	sv_matte_ptr3->SetKa(0.5);
	sv_matte_ptr3->SetKd(0.35);
	sv_matte_ptr3->SetCd(checker_ptr3);

	Box* box_ptr3 = new Box(Point3D(8, 0, -49), Point3D(18, 15, -48));
	box_ptr3->SetMaterial(sv_matte_ptr3);
	AddObject(box_ptr3);


	// ground plane

	Checker3D* checker_ptr = new Checker3D;
	checker_ptr->SetSize(8.0);
	checker_ptr->SetColor1(RGBColor(0.25, 0.25, 0.25));  			// gray
	checker_ptr->SetColor2(WHITE);	 

	SV_Matte* sv_matte_ptr = new SV_Matte;		
	sv_matte_ptr->SetKa(0.5);
	sv_matte_ptr->SetKd(0.35);
	sv_matte_ptr->SetCd(checker_ptr);	

	Plane* plane_ptr1 = new Plane(Point3D(0, 0, 0), Normal(0, 1, 0));
	plane_ptr1->SetMaterial(sv_matte_ptr);
	AddObject(plane_ptr1);
}

}