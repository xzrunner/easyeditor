#include "SphericalMap.h"

#include "maths/Point3D.h"
#include "utilities/Constants.h"

namespace rt
{

void SphericalMap::GetTexelCoordinates(const Point3D& local_hit_point, int xres, 
									   int yres, int& row, int& column) const
{
	// first, compute theta and phi

	float theta = acos(local_hit_point.y);
	float phi   = atan2(local_hit_point.x, local_hit_point.z);
	if (phi < 0.0) {
		phi += TWO_PI;
	}

	// next, map theta and phi to (u, v) in [0, 1] X [0, 1]

	float u = phi * INV_TWO_PI;    
	float v = 1.0 - theta * INV_PI;           

	// finally, map u and v to the texel coordinates

	column 	= (int) ((xres - 1) * u);   	// column is across
	row 	= (int)  ((yres - 1) * v);    	// row is up
}

}