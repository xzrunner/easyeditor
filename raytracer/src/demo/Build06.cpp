#include "world/World.h"
#include "tracer/RayCast.h"
#include "cameras/ThinLens.h"
#include "lights/PointLight.h"
#include "samplers/MultiJittered.h"
#include "objects/Grid.h"
#include "objects/Box.h"
#include "objects/Plane.h"
#include "texture/Checker3D.h"
#include "maths/maths.h"
#include "materials/Matte.h"
#include "materials/SV_Matte.h"
#include "utilities/Constants.h"

namespace rt
{

void World::Build(void) 
{
	int num_samples = 100;

	m_vp.SetWidth(600);			
	m_vp.SetHeight(400);
	m_vp.SetSamples(num_samples);
	m_vp.SetPixelSize(1.0);
	m_vp.SetMaxDepth(0);

	m_background_color = BLACK;
	m_tracer = new RayCast(this);	

	// thin lens camera	

	ThinLens* thin_lens_ptr = new ThinLens;
	thin_lens_ptr->SetEye(Point3D(100, 100, 50)); 
	thin_lens_ptr->SetLookat(Point3D(0, -10, 0));
	thin_lens_ptr->SetViewDistance(390.0); 
	thin_lens_ptr->SetFocalDistance(135.0);
	thin_lens_ptr->SetLensRadius(5.0); 			
	thin_lens_ptr->SetSampler(new MultiJittered(num_samples));		
	thin_lens_ptr->ComputeUVW();     
	m_camera = thin_lens_ptr;

	PointLight* light = new PointLight;
	light->SetLocation(Point3D(150, 500, 300));  
	light->ScaleRadiance(3.75);  
	light->SetShadows(true);
	AddLight(light);

	// city parameters

	float 	a					= 10;   // city block width:  xw extent
	float 	b   				= 12;	// city block length:  yw extent
	int 	num_rows			= 10;  	// number of blocks in the xw direction
	int 	num_columns			= 12; 	// number of blocks in the zw direction
	float	width				= 7;	// building width: xw extent in range [min, a - offset]
	float 	length				= 7;	// building length: zw extent in range [min, b - offset]
	float 	min_size			= 6;	// mininum building extent in xw and yw directions
	float 	offset				= 1.0;	// half the minimum distance between buildings
	float 	min_height			= 0.0; 	// minimum building height
	float 	max_height			= 30; 	// maximum bulding height
	float 	height;						// the building height in range [min_height, max_height]
	int		num_park_rows		= 4;  	// number of blocks of park in xw direction
	int		num_park_columns	= 6;  	// number of blocks of park in xw direction
	int 	row_test;					// there are no buildings in the park
	int 	column_test;				// there are no buildings in the park
	float 	min_color			= 0.1;  // prevents colors that are too dark
	float 	max_color			= 0.9;	// prevents colors that are too saturated

	set_rand_seed(15);  				// As the buildings' dimensions and colors are random, it's necessary to 
	// seed rand to keep these quantities the same at each run
	// If you leave this out, and change the number of samples per pixel,
	// these will change

	// The buildings are stored in a grid for efficiency, but you can render them without the grid
	// by storing them directly in the world.

	Grid* grid_ptr = new Grid;

	for (int r = 0; r < num_rows; r++) { 			// xw direction
		for (int c = 0; c < num_columns; c++) {		// zw direction
			// determine if the block is in the park

			if ((r - num_rows / 2) >= 0)
				row_test = r -  num_rows / 2;
			else
				row_test = r -  num_rows / 2 + 1;

			if ((c - num_columns / 2) >= 0)
				column_test = c - num_columns / 2;
			else
				column_test = c - num_columns / 2 + 1;

			if (abs(row_test) >= (num_park_rows / 2) || abs(column_test) >= (num_park_columns / 2)) {

				// because both matte_ptr and reflective_ptr call randf, we have to keep one of
				// them commented out to keep the boxes and colours the same for a given seed

				Matte* matte_ptr = new Matte;			
				matte_ptr->SetCd(RGBColor(
					min_color + rand_float() * (max_color - min_color), 
					min_color + rand_float() * (max_color - min_color), 
					min_color + rand_float() * (max_color - min_color)
					));
				matte_ptr->SetKa(0.4); 
				matte_ptr->SetKd(0.6); 			 

				// block center coordinates

				float xc = a * (r - num_rows / 2.0 + 0.5);
				float zc = b * (c - num_columns / 2.0 + 0.5);

				width = min_size + rand_float() * (a - 2 * offset - min_size);
				length = min_size + rand_float() * (b - 2 * offset - min_size);


				// minimum building coordinates

				float xmin = xc - width / 2.0;
				float ymin = 0.0;
				float zmin = zc - length / 2.0;

				// maximum building coordinates

				height = min_height + rand_float() * (max_height - min_height);


				// The following is a hack to make the middle row and column of buildings higher
				// on average than the other buildings. 
				// This only works when there are three rows and columns of buildings

				if (r == 1 || r == num_rows - 2 || c == 1 || c == num_columns - 2)
					height *= 1.5;

				float xmax = xc + width / 2.0;
				float ymax = height;
				float zmax = zc + length / 2.0;

				Box* building_ptr = new Box(Point3D(xmin, ymin, zmin), Point3D(xmax, ymax, zmax));
				building_ptr->SetMaterial(matte_ptr);
				grid_ptr->AddObject(building_ptr);
			}
		}
	}

	grid_ptr->SetupCells();
	AddObject(grid_ptr);


	// render the park with small green checkers

	Checker3D* checker3D_ptr1 = new Checker3D;
	checker3D_ptr1->SetSize(5.0); 
	checker3D_ptr1->SetColor1(RGBColor(0.35, 0.75, 0.35));  
	checker3D_ptr1->SetColor2(RGBColor(0.3, 0.5, 0.3));

	SV_Matte* sv_matte_ptr1 = new SV_Matte;		
	sv_matte_ptr1->SetKa(0.3);
	sv_matte_ptr1->SetKd(0.50);  
	sv_matte_ptr1->SetCd(checker3D_ptr1);

	Box* park_ptr = new Box( 	Point3D(-a * num_park_rows / 2, 0.0, -b * num_park_columns / 2), 
		Point3D(a * num_park_rows / 2, 0.1, b * num_park_columns / 2)  );										
	park_ptr->SetMaterial(sv_matte_ptr1);
	AddObject(park_ptr);


	// ground plane with checker:

	Checker3D* checker3D_ptr2 = new Checker3D;
	checker3D_ptr2->SetSize(50.0); 
	checker3D_ptr2->SetColor1(RGBColor(0.7f, 0.7f, 0.7f));  
	checker3D_ptr2->SetColor2(RGBColor(1, 1, 1));

	SV_Matte* sv_matte_ptr2 = new SV_Matte;		
	sv_matte_ptr2->SetKa(0.30);
	sv_matte_ptr2->SetKd(0.40);  
	sv_matte_ptr2->SetCd(checker3D_ptr2);

	Plane* plane_ptr = new Plane(Point3D(0, 0.01, 0), Normal(0, 1, 0));
	plane_ptr->SetMaterial(sv_matte_ptr2);
	AddObject(plane_ptr);
}

}