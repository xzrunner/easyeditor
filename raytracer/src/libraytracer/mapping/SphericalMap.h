#ifndef _RT_SPHERICAL_MAPPING_H_
#define _RT_SPHERICAL_MAPPING_H_

#include "Mapping.h"

namespace rt
{

class SphericalMap : public Mapping
{
public:
	
	virtual void GetTexelCoordinates(const Point3D& hit_point, 
		int xres, int yres, int& row, int& column) const;

}; // SphericalMap

}

#endif // _RT_SPHERICAL_MAPPING_H_