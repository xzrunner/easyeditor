#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "cameras/FishEye.h"
#include "lights/PointLight.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
#include "objects/Box.h"
#include "objects/Plane.h"
#include "objects/Grid.h"
#include "objects/Instance.h"
#include "objects/Sphere.h"
#include "mapping/SphericalMap.h"
#include "texture/Checker3D.h"
#include "texture/Image.h"
#include "texture/ImageTexture.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 1;
	
	m_vp.SetWidth(600);
	m_vp.SetHeight(600);
	m_vp.SetSamples(num_samples);
	
	m_tracer = new RayCast(this);
	
	// pinhole camera for Figure 11.7(a)
			
	Pinhole* pinhole = new Pinhole;
	pinhole->SetEye(Point3D(250, 300, 150)); 
	pinhole->SetLookat(Point3D(-20, 300, -110));  
	pinhole->SetViewDistance(250);
	pinhole->ComputeUVW();  
//	set_camera(pin_hole);
		
	
	// fisheye camera for the other parts
	
	FishEye* fisheye = new FishEye;
	
	// for parts (b), (c), (d)
	
	fisheye->SetEye(Point3D(250, 300, 150));
	fisheye->SetLookat(Point3D(-20, 300, -110));
	fisheye->SetFov(120);  // part (b)
//	fisheye->SetFov(180);  // part (c)
//	fisheye->SetFov(360);  // part (d)
   

/*	
	
	// for part (e)
	
	fisheye_ptr->set_eye(250, 450, 150); 
	fisheye_ptr->set_lookat(-20, 250, -110);  
	fisheye_ptr->set_fov(360);
	
*/


/*	
	// for part (f)
	// for this image the skydome is the only object in the scene
	// you need to comment out the two statements:
	// add_object(grid_ptr);
	// add_object(plane_ptr);
	
	fisheye_ptr->set_eye(0, 0, 0);     
	fisheye_ptr->set_lookat(0, 1, 0);
	fisheye_ptr->set_fov(180);
*/	
		
	fisheye->ComputeUVW();  
	m_camera = fisheye;


	PointLight* light = new PointLight;
	light->SetLocation(Point3D(150, 200, 65));  
	light->ScaleRadiance(5.25);
	light->SetShadows(true);
	AddLight(light);
	
	
	// box materials
		
	Matte* matte1 = new Matte;			
	matte1->SetCd(RGBColor(0, 0.5, 0.5));     // cyan
	matte1->SetKa(0.4); 
	matte1->SetKd(0.5); 
	
	Matte* matte2 = new Matte;			
	matte2->SetCd(RGBColor(1, 0, 0));     	  // red
	matte2->SetKa(0.4); 
	matte2->SetKd(0.5);  
	
	Matte* matte3 = new Matte;			
	matte3->SetCd(RGBColor(0.5, 0.6, 0));     // green
	matte3->SetKa(0.4); 
	matte3->SetKd(0.5); 
	
	
	// Construct rows of boxes stored in a grid
		
	Grid* grid_ptr = new Grid;
	
	// first row
	
	int num_boxes = 40;
	float wx = 50;
	float wz = 50;
	float h = 150;
	float s = 100;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box = new Box(	Point3D(-wx, 0, -(j + 1) * wz - j * s), 
								Point3D(0, h, - j * wz - j * s));
		box->SetMaterial(matte1);
	//	add_object(box_ptr);
		grid_ptr->AddObject(box);
	}
	
	
	// second row
	
	h = 300;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box = new Box(	Point3D(-wx -wx - s, 0, -(j + 1) * wz - j * s), 
								Point3D(-wx - s, h, - j * wz - j * s));
		box->SetMaterial(matte2);
//		add_object(box_ptr);
		grid_ptr->AddObject(box);
	}
	
	
	// third row
	
	h = 850; 
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box = new Box(	Point3D(-wx - 2 * wx - 2 * s, 0, -(j + 1) * wz - j * s), 
								Point3D(-2 * wx - 2 * s, h, - j * wz - j * s));
		box->SetMaterial(matte3);
//		add_object(box_ptr);
		grid_ptr->AddObject(box);
	}
	
	
	// a column
	
	h = 150;
	
	for (int j = 0; j < num_boxes; j++) {
		Box* box = new Box(	Point3D(-3 * (wx + s) - (j + 1) * wz - j * s, 0, -wx), 
								Point3D(-3 * (wx + s) - j * wz - j * s, h, 0));
		box->SetMaterial(matte1);
//		add_object(box_ptr);
		grid_ptr->AddObject(box);
	}
	
	grid_ptr->SetupCells();
	AddObject(grid_ptr);
	
	
	// ground plane with checker
	
	Checker3D* checker = new Checker3D;
	checker->SetSize(2 * wx); 
	checker->SetColor1(RGBColor(0.7, 0.7, 0.7));  
	checker->SetColor2(WHITE);
	
	SV_Matte* sv_matte = new SV_Matte;		
	sv_matte->SetKa(0.20);
	sv_matte->SetKd(0.50); 
	sv_matte->SetCd(checker);	
	Plane* plane_ptr = new Plane(Point3D(0, 1, 0), Normal(0, 1, 0));
	plane_ptr->SetMaterial(sv_matte);
	AddObject(plane_ptr);

	
	// skydome with clouds
	
	Image* image = new Image;						
	image->ReadPPMFile("CloudsSmall.ppm");
	
	SphericalMap* spherical_map = new SphericalMap; 
	
	ImageTexture* image_texture = new ImageTexture(image); 
	image_texture->SetMapping(spherical_map);
	
	SV_Matte* sv_matte2 = new SV_Matte;		
	sv_matte2->SetKa(1);
	sv_matte2->SetKd(0.85);
	sv_matte2->SetCd(image_texture);	
	
	Instance* sphere = new Instance(new Sphere); 
	sphere->Scale(Vector3D(1000000, 1000000, 1000000));
	sphere->SetMaterial(sv_matte2);
	AddObject(sphere);
}

}