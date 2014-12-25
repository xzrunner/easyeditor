#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "lights/Directional.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
#include "objects/Grid.h"
#include "objects/Box.h"
#include "objects/Plane.h"
#include "texture/Checker3D.h"
#include "utilities/RGBColor.h"

namespace rt
{

void World::Build()
{
	m_vp.SetWidth(400);			
	m_vp.SetHeight(400);
	m_vp.SetSamples(25);
	
	m_tracer = new RayCast(this);
			
	Pinhole* camera = new Pinhole;
	
	// Figure9.10(a)

	camera->SetEye(Point3D(150, 195, 125));
	camera->SetLookat(Point3D(0, 195, -40));
	camera->SetViewDistance(167);

// 	// Figure9.10(b)
// 		
// 	camera->SetEye(Point3D(150, 300, 125));   
// 	camera->SetLookat(Point3D(0, 265, -40));  
// 	camera->SetViewDistance(167);

// 	// Figure9.10(c)
// 		
// 	camera->SetEye(Point3D(-250, 350, 500));   
// 	camera->SetLookat(Point3D(-250, 350, 0));  
// 	camera->SetViewDistance(280);

	camera->ComputeUVW(); 
	m_camera = camera;

	
	Directional* light = new Directional;
	light->SetDirection(Vector3D(150, 200, 65));   
	light->ScaleRadiance(5.0);
	light->SetShadows(true);
	AddLight(light);
	
	
	Matte* matte1 = new Matte;			
	matte1->SetCd(RGBColor(0, 0.5, 0.5));     // cyan
	matte1->SetKa(0.4); 
	matte1->SetKd(0.5); 
	
	Matte* matte_ptr2 = new Matte;			
	matte_ptr2->SetCd(RGBColor(0.8, 0.5, 0));     // orange
	matte_ptr2->SetKa(0.4); 
	matte_ptr2->SetKd(0.5);  
	
	Matte* matte_ptr3 = new Matte;			
	matte_ptr3->SetCd(RGBColor(0.5, 0.6, 0));     // green
	matte_ptr3->SetKa(0.4); 
	matte_ptr3->SetKd(0.5); 
	
	
	// construct rows of boxes parallel to the zw axis
	
	Grid* grid = new Grid;
	
	// first row
	
	int num_boxes = 40;
	float wx = 50;
	float wz = 50;
	float h = 150;
	float s = 100;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box_ptr = new Box(	Point3D(-wx, 0, -(j + 1) * wz - j * s), 
								Point3D(0, h, - j * wz - j * s));
		box_ptr->SetMaterial(matte_ptr2);
	//	AddObject(box_ptr);
		grid->AddObject(box_ptr);
	}
	
	
	// second row
	
	h = 300;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box_ptr = new Box(	Point3D(-wx -wx - s, 0, -(j + 1) * wz - j * s), 
								Point3D(-wx - s, h, - j * wz - j * s));
		box_ptr->SetMaterial(matte1);
//		AddObject(box_ptr);
		grid->AddObject(box_ptr);
	}
	
	
	// third row
	
	h = 600;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box_ptr = new Box(	Point3D(-wx - 2 * wx - 2 * s, 0, -(j + 1) * wz - j * s), 
								Point3D(-2 * wx - 2 * s, h, - j * wz - j * s));
		box_ptr->SetMaterial(matte_ptr3);
//		AddObject(box_ptr);
		grid->AddObject(box_ptr);
	}
	
	
	// a column
	
	h = 150;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box_ptr = new Box(	Point3D(-3 * (wx + s) - (j + 1) * wz - j * s, 0, -wx), 
								Point3D(-3 * (wx + s) - j * wz - j * s, h, 0));
		box_ptr->SetMaterial(matte_ptr2);
//		AddObject(box_ptr);
		grid->AddObject(box_ptr);
	}
	
	grid->SetupCells();
	AddObject(grid);
	
	
	// ground plane with checker:
	
	Checker3D* checker3D = new Checker3D;
	checker3D->SetSize(wx); 
	checker3D->SetColor1(RGBColor(0.7, 0.7, 0.7));  
	checker3D->SetColor2(RGBColor(1, 1, 1));

	SV_Matte* sv_matte_ptr = new SV_Matte;
	sv_matte_ptr->SetKa(0.25);
	sv_matte_ptr->SetKd(0.35);
	sv_matte_ptr->SetCd(checker3D);
	
	Plane* plane_ptr = new Plane(Point3D(0, 1, 0), Normal(0, 1, 0));
	plane_ptr->SetMaterial(sv_matte_ptr);
	AddObject(plane_ptr);	
}

}