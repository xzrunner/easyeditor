#include "World.h"

#include "lights/Ambient.h"
#include "lights/Directional.h"
#include "lights/PointLight.h"
#include "tracer/RayCast.h"
#include "cameras/Pinhole.h"
#include "materials/Matte.h"
#include "objects/Sphere.h"
#include "objects/Plane.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build()
{
	int num_samples = 5; 

	// view plane  

	m_vp.SetWidth(400);
	m_vp.SetHeight(400);
	m_vp.SetPixelSize(0.5);
	m_vp.SetSamples(num_samples);

	// the ambient light here is the same as the default set in the World
	// constructor, and can therefore be left out

	Ambient* ambient = new Ambient;
	ambient->ScaleRadiance(1);
	m_ambient = ambient;

	m_background_color = BLACK;			// default color - this can be left out

	m_tracer = new RayCast(this);


	// camera

	Pinhole* pinhole = new Pinhole;
	pinhole->SetEye(Point3D(0, 0, 500)); 
	pinhole->SetLookat(Point3D(0, 0, 0));
	pinhole->SetViewDistance(600.0);
	pinhole->ComputeUVW();
	m_camera = pinhole;

	// light

//  	Directional* light = new Directional;
//  	light->SetDirection(Vector3D(100, 100, 200));
//  	light->ScaleRadiance(3.0); 	
//  	AddLight(light);

 	PointLight* light = new PointLight();
 	light->SetLocation(Point3D(-100, -100, 200));
 	light->ScaleRadiance(5.0); 
 	AddLight(light);

	// 	Directional* light2 = new Directional;
	// 	light1->set_direction(-100, -100, -200);
	// 	light2->scale_radiance(3);
	// 	add_light(light2);
	// 	
	// colors

	RGBColor yellow(1, 1, 0);										// yellow
	RGBColor brown(0.71f, 0.40f, 0.16f);								// brown
	RGBColor darkGreen(0.0f, 0.41f, 0.41f);							// darkGreen
	RGBColor orange(1, 0.75f, 0);									// orange
	RGBColor green(0, 0.6f, 0.3f);									// green
	RGBColor lightGreen(0.65f, 1, 0.30f);								// light green
	RGBColor darkYellow(0.61f, 0.61f, 0);								// dark yellow
	RGBColor lightPurple(0.65f, 0.3f, 1);								// light purple
	RGBColor darkPurple(0.5f, 0, 1);									// dark purple
	RGBColor grey(0.25f, 0.25f, 0.25f);								// grey


	// Matte material reflection coefficients - common to all materials

	float ka = 0.25;
	float kd = 0.75;


	// spheres

	Matte* matte1 = new Matte;   
	matte1->SetKa(ka);	
	matte1->SetKd(kd);
	matte1->SetCd(yellow);				
	Sphere*	sphere1 = new Sphere(Point3D(70, 3, 0), 30); 
	sphere1->SetMaterial(matte1);	   							// yellow
	AddObject(sphere1);

	Matte* matte2 = new Matte;
	matte2->SetKa(ka);	
	matte2->SetKd(kd);
	matte2->SetCd(brown);
	Sphere*	sphere2 = new Sphere(Point3D(45, -7, -60), 20); 
	sphere2->SetMaterial(matte2);								// brown
	AddObject(sphere2);


	Matte* matte3 = new Matte;
	matte3->SetKa(ka);	
	matte3->SetKd(kd);
	matte3->SetCd(darkGreen);	
	Sphere*	sphere3 = new Sphere(Point3D(40, 43, -100), 17); 
	sphere3->SetMaterial(matte3);								// dark green
	AddObject(sphere3);

	Matte* matte4 = new Matte;
	matte4->SetKa(ka);	
	matte4->SetKd(kd);
	matte4->SetCd(orange);
	Sphere*	sphere4 = new Sphere(Point3D(-20, 28, -15), 20); 
	sphere4->SetMaterial(matte4);								// orange
	AddObject(sphere4);

	Matte* matte5 = new Matte;
	matte5->SetKa(ka);	
	matte5->SetKd(kd);
	matte5->SetCd(green);
	Sphere*	sphere5 = new Sphere(Point3D(-25, -7, -35), 27); 			
	sphere5->SetMaterial(matte5);								// green
	AddObject(sphere5);

	Matte* matte6 = new Matte;
	matte6->SetKa(ka);	
	matte6->SetKd(kd);
	matte6->SetCd(lightGreen);
	Sphere*	sphere6 = new Sphere(Point3D(20, -27, -35), 25); 
	sphere6->SetMaterial(matte6);								// light green
	AddObject(sphere6);

	Matte* matte7 = new Matte;
	matte7->SetKa(ka);	
	matte7->SetKd(kd);
	matte7->SetCd(green);
	Sphere*	sphere7 = new Sphere(Point3D(35, 18, -35), 22); 
	sphere7->SetMaterial(matte7);   							// green
	AddObject(sphere7);

	Matte* matte8 = new Matte;
	matte8->SetKa(ka);	
	matte8->SetKd(kd);
	matte8->SetCd(brown);
	Sphere*	sphere8 = new Sphere(Point3D(-57, -17, -50), 15);  
	sphere8->SetMaterial(matte8);								// brown
	AddObject(sphere8);

	Matte* matte9 = new Matte;
	matte9->SetKa(ka);	
	matte9->SetKd(kd);
	matte9->SetCd(lightGreen);
	Sphere*	sphere9 = new Sphere(Point3D(-47, 16, -80), 23); 
	sphere9->SetMaterial(matte9);								// light green
	AddObject(sphere9);

	Matte* matte10 = new Matte;
	matte10->SetKa(ka);	
	matte10->SetKd(kd);
	matte10->SetCd(darkGreen);	
	Sphere*	sphere10 = new Sphere(Point3D(-15, -32, -60), 22); 
	sphere10->SetMaterial(matte10);     						// dark green
	AddObject(sphere10);

	Matte* matte11 = new Matte;
	matte11->SetKa(ka);	
	matte11->SetKd(kd);
	matte11->SetCd(darkYellow);
	Sphere*	sphere11 = new Sphere(Point3D(-35, -37, -80), 22); 
	sphere11->SetMaterial(matte11);							// dark yellow
	AddObject(sphere11);

	Matte* matte12 = new Matte;
	matte12->SetKa(ka);	
	matte12->SetKd(kd);
	matte12->SetCd(darkYellow);
	Sphere*	sphere12 = new Sphere(Point3D(10, 43, -80), 22); 
	sphere12->SetMaterial(matte12);							// dark yellow
	AddObject(sphere12);

	Matte* matte13 = new Matte;
	matte13->SetKa(ka);	
	matte13->SetKd(kd);
	matte13->SetCd(darkYellow);		
	Sphere*	sphere13 = new Sphere(Point3D(30, -7, -80), 10); 
	sphere13->SetMaterial(matte13);
	AddObject(sphere13);											// dark yellow (hidden)

	Matte* matte14 = new Matte;
	matte14->SetKa(ka);	
	matte14->SetKd(kd);
	matte14->SetCd(darkGreen);	
	Sphere*	sphere14 = new Sphere(Point3D(-40, 48, -110), 18); 
	sphere14->SetMaterial(matte14); 							// dark green
	AddObject(sphere14);

	Matte* matte15 = new Matte;
	matte15->SetKa(ka);	
	matte15->SetKd(kd);
	matte15->SetCd(brown);	
	Sphere*	sphere15 = new Sphere(Point3D(-10, 53, -120), 18); 
	sphere15->SetMaterial(matte15); 							// brown
	AddObject(sphere15);

	Matte* matte16 = new Matte;
	matte16->SetKa(ka);	
	matte16->SetKd(kd);
	matte16->SetCd(lightPurple);
	Sphere*	sphere16 = new Sphere(Point3D(-55, -52, -100), 10); 
	sphere16->SetMaterial(matte16);							// light purple
	AddObject(sphere16);

	Matte* matte17 = new Matte;
	matte17->SetKa(ka);	
	matte17->SetKd(kd);
	matte17->SetCd(brown);
	Sphere*	sphere17 = new Sphere(Point3D(5, -52, -100), 15); 		
	sphere17->SetMaterial(matte17);							// browm
	AddObject(sphere17);

	Matte* matte18 = new Matte;
	matte18->SetKa(ka);	
	matte18->SetKd(kd);
	matte18->SetCd(darkPurple);
	Sphere*	sphere18 = new Sphere(Point3D(-20, -57, -120), 15); 
	sphere18->SetMaterial(matte18);							// dark purple
	AddObject(sphere18);

	Matte* matte19 = new Matte;
	matte19->SetKa(ka);	
	matte19->SetKd(kd);
	matte19->SetCd(darkGreen);
	Sphere*	sphere19 = new Sphere(Point3D(55, -27, -100), 17); 
	sphere19->SetMaterial(matte19);							// dark green
	AddObject(sphere19);

	Matte* matte20 = new Matte;
	matte20->SetKa(ka);	
	matte20->SetKd(kd);
	matte20->SetCd(brown);
	Sphere*	sphere20 = new Sphere(Point3D(50, -47, -120), 15); 
	sphere20->SetMaterial(matte20);							// browm
	AddObject(sphere20);

	Matte* matte21 = new Matte;
	matte21->SetKa(ka);	
	matte21->SetKd(kd);
	matte21->SetCd(lightPurple); 	
	Sphere*	sphere21 = new Sphere(Point3D(70, -42, -150), 10); 
	sphere21->SetMaterial(matte21);							// light purple
	AddObject(sphere21);

	Matte* matte22 = new Matte;
	matte22->SetKa(ka);	
	matte22->SetKd(kd);
	matte22->SetCd(lightPurple);
	Sphere*	sphere22 = new Sphere(Point3D(5, 73, -130), 12); 
	sphere22->SetMaterial(matte22);							// light purple
	AddObject(sphere22);

	Matte* matte23 = new Matte;
	matte23->SetKa(ka);	
	matte23->SetKd(kd);
	matte23->SetCd(darkPurple);
	Sphere*	sphere23 = new Sphere(Point3D(66, 21, -130), 13); 			
	sphere23->SetMaterial(matte23);							// dark purple
	AddObject(sphere23);	

	Matte* matte24 = new Matte;
	matte24->SetKa(ka);	
	matte24->SetKd(kd);
	matte24->SetCd(lightPurple);  
	Sphere*	sphere24 = new Sphere(Point3D(72, -12, -140), 12); 
	sphere24->SetMaterial(matte24);							// light purple
	AddObject(sphere24);

	Matte* matte25 = new Matte;
	matte25->SetKa(ka);	
	matte25->SetKd(kd);
	matte25->SetCd(green);
	Sphere*	sphere25 = new Sphere(Point3D(64, 5, -160), 11); 			
	sphere25->SetMaterial(matte25);					 		// green
	AddObject(sphere25);

	Matte* matte26 = new Matte;
	matte26->SetKa(ka);	
	matte26->SetKd(kd);
	matte26->SetCd(lightPurple);
	Sphere*	sphere26 = new Sphere(Point3D(55, 38, -160), 12); 		
	sphere26->SetMaterial(matte26);							// light purple
	AddObject(sphere26);

	Matte* matte27 = new Matte;
	matte27->SetKa(ka);	
	matte27->SetKd(kd);
	matte27->SetCd(lightPurple);
	Sphere*	sphere27 = new Sphere(Point3D(-73, -2, -160), 12); 		
	sphere27->SetMaterial(matte27);							// light purple
	AddObject(sphere27);

	Matte* matte28 = new Matte;
	matte28->SetKa(ka);	
	matte28->SetKd(kd);
	matte28->SetCd(darkPurple);
	Sphere*	sphere28 = new Sphere(Point3D(30, -62, -140), 15); 
	sphere28->SetMaterial(matte28); 							// dark purple
	AddObject(sphere28);

	Matte* matte29 = new Matte;
	matte29->SetKa(ka);	
	matte29->SetKd(kd);
	matte29->SetCd(darkPurple);
	Sphere*	sphere29 = new Sphere(Point3D(25, 63, -140), 15); 
	sphere29->SetMaterial(matte29);							// dark purple
	AddObject(sphere29);

	Matte* matte30 = new Matte;
	matte30->SetKa(ka);	
	matte30->SetKd(kd);
	matte30->SetCd(darkPurple);
	Sphere*	sphere30 = new Sphere(Point3D(-60, 46, -140), 15);  
	sphere30->SetMaterial(matte30); 							// dark purple
	AddObject(sphere30);

	Matte* matte31 = new Matte;
	matte31->SetKa(ka);	
	matte31->SetKd(kd);
	matte31->SetCd(lightPurple);
	Sphere*	sphere31 = new Sphere(Point3D(-30, 68, -130), 12); 
	sphere31->SetMaterial(matte31); 							// light purple
	AddObject(sphere31);

	Matte* matte32 = new Matte;
	matte32->SetKa(ka);	
	matte32->SetKd(kd);
	matte32->SetCd(green);
	Sphere*	sphere32 = new Sphere(Point3D(58, 56, -180), 11);   
	sphere32->SetMaterial(matte32);							//  green
	AddObject(sphere32);

	Matte* matte33 = new Matte;
	matte33->SetKa(ka);	
	matte33->SetKd(kd);
	matte33->SetCd(green);
	Sphere*	sphere33 = new Sphere(Point3D(-63, -39, -180), 11); 
	sphere33->SetMaterial(matte33);							// green 
	AddObject(sphere33);

	Matte* matte34 = new Matte;
	matte34->SetKa(ka);	
	matte34->SetKd(kd);
	matte34->SetCd(lightPurple);
	Sphere*	sphere34 = new Sphere(Point3D(46, 68, -200), 10); 	
	sphere34->SetMaterial(matte34);							// light purple
	AddObject(sphere34);

	Matte* matte35 = new Matte;
	matte35->SetKa(ka);	
	matte35->SetKd(kd);
	matte35->SetCd(lightPurple);
	Sphere*	sphere35 = new Sphere(Point3D(-3, -72, -130), 12); 
	sphere35->SetMaterial(matte35);							// light purple
	AddObject(sphere35);


	// vertical plane

	Matte* matte36 = new Matte;
	matte36->SetKa(ka);	
	matte36->SetKd(kd);
	matte36->SetCd(grey);
	Plane* plane = new Plane(Point3D(0, 0, -150), Normal(0, 0, 1));
	plane->SetMaterial(matte36);
	AddObject (plane);
}

}