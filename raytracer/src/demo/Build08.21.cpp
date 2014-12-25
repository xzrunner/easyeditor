#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "lights/Directional.h"
#include "texture/Checker3D.h"
#include "materials/SV_Matte.h"
#include "objects/Sphere.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 1;
	
	m_vp.SetWidth(500);			
	m_vp.SetHeight(500);
	m_vp.SetSamples(num_samples);
	m_vp.SetPixelSize(1.0);
	
	m_background_color = BLACK;
	m_tracer = new RayCast(this);
	
	
	Pinhole* pinhole = new Pinhole;
	
// 	pinhole->SetEye(Point3D(0, 15, 0)); 		// for Figure 8.21(a)
// 	pinhole->SetViewDistance(755);
		
// 	pinhole->SetEye(Point3D(0, 5, 0)); 			// for Figure 8.21(b)
// 	pinhole->SetViewDistance(235);
		
	pinhole->SetEye(Point3D(0, 2, 0)); 			// for Figure 8.21(c)
	pinhole->SetViewDistance(73.0);
	
	pinhole->SetLookat(Point3D(0, 0, 0));
	pinhole->ComputeUVW(); 
	m_camera = pinhole;
	
		
	Directional* light = new Directional;
 	light->SetDirection(Vector3D(0, 1, 1));
	light->ScaleRadiance(3.0);
	AddLight(light);
	
	// construct the spheres
	
	float d = 2.0; 		// sphere center spacing 
	float r = 0.75; 	// sphere radius
	float xc, yc; 		// sphere center coordinates
	int num_rows = 5;
	int num_columns = 5;
	
	Checker3D* checker = new Checker3D;
	checker->SetSize(0.5);
	checker->SetColor1(RGBColor(0,0.4,0.8));
	checker->SetColor2(RGBColor(1,1,1));
			
	SV_Matte* sv_matte = new SV_Matte;		
	sv_matte->SetKa(0.2);
	sv_matte->SetKd(0.8);
	sv_matte->SetCd(checker);
	
	for (int k = 0; k < num_columns; k++) {		// up
		for (int j = 0; j < num_rows; j++) {	// across
				Sphere*	sphere = new Sphere();
				xc = d * (j - (num_columns - 1) / 2.0);
				yc = d * (k - (num_rows - 1) / 2.0);
				sphere->SetCenter(Point3D(xc, 0, yc));
				sphere->SetRadius(r);		
				sphere->SetMaterial(sv_matte);
				AddObject(sphere);
		}	
	}
}

}